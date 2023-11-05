#include "Request/Request.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request(std::vector<unsigned char> rawData) : _rawData(rawData) {
  _header.contentLength = 0;
  setValidationStatus(0);
  // setAutoIndex("on"); // to help avoid unit value error
  parseRequestData();
}

// Request::Request(std::vector<unsigned char> rawData, ServerConfig server) : _rawData(rawData) {
//   std::istringstream iss(rawData);
//   std::string requestMethod;
//   std::string requestTarget;
//   bool hasMatchingLocation = false;
//   Location location;

//   iss >> requestMethod;
//   iss >> requestTarget;

//   for (size_t i = 0; i < server.locations.size(); i++) {
//     location = server.locations[i];
//     if (std::find(location.methods.begin(), location.methods.end(),
//                   requestMethod) != location.methods.end() &&
//         location.url == requestTarget) {
//       this->_autoIndex = location.autoindex;
//       this->_root = location.root;
//       this->_redirect = location.redirect;
//       this->_index = location.index;
//       this->_errorPages = location.error_page;
//       hasMatchingLocation = true;
//       this->_validationStatus = VALID_REQUEST;
//       break;
//     }
//     if (location.url == requestTarget &&
//         std::find(location.methods.begin(), location.methods.end(),
//                   requestMethod) == location.methods.end()) {
//       this->_validationStatus = INVALID_METHOD;
//     }
//   }
//   if (!hasMatchingLocation) {
//     this->_validationStatus = INVALID_LOCATION;
//   }
//   parseRequestData();
// }

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Request::~Request() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

std::ostream& operator<<(std::ostream& o, Request &i) {
  o << i.getCharRawData();
  return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void Request::parseRequestData() {
  parseRawHeader();
  parseHeaderProperties();
  validate();
  //parseBody();
}

void Request::parseRawHeader() {
  std::string rawData(reinterpret_cast<char*>(_rawData.data()), _rawData.size());
  // std::string rawData((char *)_rawData.data());
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

/* void Request::parseBody() {
  if (this->getHeaderContentLength() < 1 ||
      isFile(this->getHeaderContentType())) {
    return;
  }

  std::string rawData(reinterpret_cast<char*>(_rawData.data()));

  std::cout << "rawData as string=" << rawData << std::endl;

  std::endl(std::cout);

  std::cout << "rawData = " << std::endl;
  for(size_t i = 0; i< _rawData.size(); i++){
    std::cout << _rawData[i];
  }
} */

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
  size_t reqLen = strlen(reinterpret_cast<char *>(this->getCharRawData()));
  size_t headerSize = this->getHeaderRawDate().length() + 4;
  if (reqLen != headerSize + contentLength) {
    throw RequestValidationException();
  }

}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

char* Request::getBody() const { return _body; }

std::vector<unsigned char> Request::getRawData() { return _rawData; }

char * Request::getCharRawData() { return reinterpret_cast<char*>(_rawData.data()); }

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
