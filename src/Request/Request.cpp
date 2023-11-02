#include "Request/Request.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request(const char* rawData) : _rawData(rawData) {
  _header.contentLength = 0;
  parseRequestData();
}

#include <algorithm>

/* 
  validar a estrutura de pastas(locationUrl com requestTarget)
  validar se no request target tem o caminho para um arquivo (index.html, index.php)


 */

bool validateRequestLocation(Location location, std::string requestTarget){
  /* 
    location.url = /users
    location.url = /
    requestTarget = /users/notas

    location  url anatomy = root = / subfolder = users/
    requestTarget anatomy = root = / subfolder = users/ subfolder = notas
    comparar a url com o request target, se tudo for igual, retorna true, se não, false
    a validação da extensão so e feita apos a validação de pasta
   */

  std::vector<std::string> urlParts;
  std::string delimiter = "/";
  size_t pos = 0;
  std::string token;
  while ((pos = location.url.find(delimiter)) != std::string::npos) {
      token = location.url.substr(0, pos);
      urlParts.push_back(token);
      location.url.erase(0, pos + delimiter.length());
  }
  urlParts.push_back(location.url);

  std::vector<std::string> requestTargetParts;
  pos = 0;
  while ((pos = requestTarget.find(delimiter)) != std::string::npos) {
      token = requestTarget.substr(0, pos + 1);
      requestTargetParts.push_back(token);
      requestTarget.erase(0, pos + delimiter.length());
  }
  requestTargetParts.push_back(requestTarget);

  size_t i = 0;
  size_t biggerSize = urlParts.size() > requestTargetParts.size() ? urlParts.size() : requestTargetParts.size();

  std::cout << "location.url " << location.url << std::endl;
  std::cout << "requestTarget " << requestTarget << std::endl;
  std::cout << "urlParts.size() " << urlParts.size() << std::endl;
  std::cout << "requestTargetParts.size() " << requestTargetParts.size() << std::endl;
  std::cout << "biggerSize " << biggerSize << std::endl;
  std::endl(std::cout);

 //print all requestTargetParts and urlParts
  for (size_t i = 0; i < urlParts.size(); i++) {
    std::cout << "urlParts[" << i << "] = " << urlParts[i] << std::endl;
  }
  std::endl(std::cout);
  for (size_t i = 0; i < requestTargetParts.size(); i++) {
    std::cout << "requestTargetParts[" << i << "] = " << requestTargetParts[i] << std::endl;
  }

  while(i < biggerSize){
    if(urlParts[i] != requestTargetParts[i]){
      break;
    }
    i++;
  }
  if(i > 2 ) {
    return true;
  }

  if(location.url.compare("/") == 0 && i > 1){
    return true;
  }

  return false;
}

Request::Request(const char* rawData, ServerConfig server) : _rawData(rawData) {
  std::istringstream iss(rawData);
  std::string requestMethod;
  std::string requestTarget;
  bool hasMatchingLocation = false;
  Location location;

  iss >> requestMethod;
  iss >> requestTarget;


  for (size_t i = 0; i < server.locations.size(); i++) {
    location = server.locations[i];
    if (validateRequestLocation(location, requestTarget) && std::find(location.methods.begin(), location.methods.end(),
                  requestMethod) != location.methods.end()) {
      this->_autoIndex = location.autoindex;
      this->_root = location.root;
      this->_redirect = location.redirect;
      this->_index = location.index;
      this->_errorPages = location.error_page;
      hasMatchingLocation = true;
      this->_validationStatus = VALID_REQUEST;
      break;
    }
    if (location.url == requestTarget &&
        std::find(location.methods.begin(), location.methods.end(),
                  requestMethod) == location.methods.end()) {
      this->_validationStatus = INVALID_METHOD;
    }
  }
  if (!hasMatchingLocation) {
    this->_validationStatus = INVALID_LOCATION;
  }
  parseRequestData();
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Request::~Request() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

std::ostream& operator<<(std::ostream& o, Request const& i) {
  o << i.getRawData();
  return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void Request::parseRequestData() {
  parseRawHeader();
  parseHeaderProperties();
  validate();
  parseBody();
}

void Request::parseRawHeader() {
  std::string rawData(_rawData);
  std::size_t delim = rawData.find(CRLF);
  if (delim == std::string::npos) {
    throw RequestValidationException::InvalidFormat();
  }

  _header.rawData = rawData.substr(0, delim);
}

void Request::parseHeaderProperties() {
  std::istringstream iss(_header.rawData);
  std::string line;

  std::getline(iss, line);
  parseRequestLine(line);

  while (std::getline(iss, line)) {
    parseGeneralHeader(line);
  }
}

void Request::parseRequestLine(std::string line) {
  std::istringstream lineStream(line);
  std::string token[3];

  for (int j = 0; j < 3; j++) {
    getline(lineStream, token[j], ' ');
  }
  _header.method = token[0];
  _header.target = token[1];
  _header.protocol = token[2].substr(0, token[2].size() - 1);
}

void Request::parseGeneralHeader(std::string line) {
  std::string parseableProperties[] = {"Host", "Content-Type",
                                       "Content-Length"};

  int listLength = sizeof(parseableProperties) / sizeof(parseableProperties[0]);

  for (int i = 0; i < listLength; i++) {
    if (line.find(parseableProperties[i]) == std::string::npos) {
      continue;
    }
    std::string value = getPropertyValueFrom(line);
    switch (i) {
      case 0:
        _header.host = value;
        break;
      case 1:
        _header.contentType = value;
        break;
      case 2:
        _header.contentLength = std::atoi(value.c_str());
        break;
      default:
        break;
    }
  }
}

std::string Request::getPropertyValueFrom(std::string line) {
  size_t colon = line.find(":");
  if (colon == std::string::npos) {
    throw RequestValidationException::InvalidFormat();
  }
  std::string propertyValue = line.substr(colon + 1);

  while (propertyValue[0] == ' ') {
    propertyValue = propertyValue.substr(1);
  }

  return propertyValue;
}

void Request::parseBody() {
  if (this->getHeaderContentLength() < 1 ||
      isFile(this->getHeaderContentType())) {
    return;
  }
  std::string rawData(_rawData);
  size_t reqLen = strlen(_rawData);
  size_t headerSize = reqLen - this->getHeaderContentLength();
  _body = new char[this->getHeaderContentLength()];
  memcpy(_body, _rawData + headerSize, this->getHeaderContentLength());
}

void Request::validate() {
  validateMethod();
  validateContentLength();
  validateBody();
}

void Request::validateMethod() {
  std::string acceptedMethods[] = {"GET", "POST", "DELETE"};
  bool validatedMethod = false;

  for (int i = 0; i < 3; i++) {
    if (_header.method.find(acceptedMethods[i]) != std::string::npos) {
      validatedMethod = true;
      break;
    }
  }
  if (!validatedMethod) {
    throw RequestValidationException::InvalidMethod(_header.method.c_str());
  }
}

void Request::validateContentLength() {
  if (_header.contentLength < 0) {
    throw RequestValidationException();
  }
}

void Request::validateBody() {
  size_t contentLength = this->getHeaderContentLength();
  std::string contentType = this->getHeaderContentType();
  if (contentLength == 0 || isFile(contentType)) {
    return;
  }
  size_t reqLen = strlen(_rawData);
  size_t headerSize = this->getHeaderRawDate().length() + 4;

  if (reqLen != headerSize + contentLength) {
    throw RequestValidationException();
  }
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

char* Request::getBody() const { return _body; }

std::string Request::getRawData() const { return _rawData; }

std::string Request::getHeaderTarget() const { return _header.target; }

std::string Request::getHeaderRawDate() const { return _header.rawData; }

std::string Request::getHeaderMethod() const { return _header.method; }

std::string Request::getHeaderProtocol() const { return _header.protocol; }

std::string Request::getHeaderHost() const { return _header.host; }

std::string Request::getHeaderContentType() const {
  return _header.contentType;
}

int Request::getHeaderContentLength() const { return _header.contentLength; }

int Request::getValidationStatus() const { return _validationStatus; }

int Request::setValidationStatus(int status) {
  return _validationStatus = status;
}

std::string Request::getServerRoot() const { return _root; }

void Request::setServerRoot(std::string root) { _root = root; }

std::string Request::getRedirect() const { return _redirect; }

void Request::setRedirect(std::string location) { _redirect = location; }

std::vector<std::string> Request::getIndex() const { return _index; }

void Request::setIndex(std::vector<std::string> indexFile) {
  _index = indexFile;
}

std::vector<ErrorPage> Request::getErrorPages() const { return _errorPages; }

void Request::setErrorPages(std::vector<ErrorPage> errorPages) {
  _errorPages = errorPages;
}

bool Request::getAutoIndex() const { return _autoIndex; }

void Request::setAutoIndex(std::string autoIndex) {
  _autoIndex = (0 == autoIndex.compare("on"));
}

/*
** --------------------------------- STATIC ---------------------------------
*/

bool isFile(std::string contentType) {
  if (!(contentType.find("multipart/form-data") == std::string::npos)) {
    return true;
  } else if (!(contentType.find("application") == std::string::npos)) {
    return true;
  }
  return false;
}

/* ************************************************************************** */