#include "Request/Request.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request(const char* rawData) : _rawData(rawData) {
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
  if (this->getHeaderContentLength() < 1) {
    return;
  }
  std::string rawData(_rawData);
  size_t reqLen = strlen(_rawData);
  size_t headerSize = reqLen - this->getHeaderContentLength();
  _body = new char[this->getHeaderContentLength()];
  memcpy(_body, _rawData + headerSize, this->getHeaderContentLength());
  // _body = bodyBuffer;
  // _body = _rawData.substr(_rawData.find(CRLF) + 4);
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
  size_t contentLength = static_cast<size_t>(this->getHeaderContentLength());

  if (contentLength == 0) {
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

std::string Request::getServerRoot() const { return _serverRoot; }

void Request::setServerRoot(std::string root) { _serverRoot = root; }

std::string Request::getRedirect() const { return _redirect; }

void Request::setRedirect(std::string location) { _redirect = location; }

std::string Request::getIndex() const { return _index; }

void Request::setIndex(std::string indexFile) { _index = indexFile; }

bool Request::getAutoIndex() const { return _autoIndex; }

void Request::setAutoIndex(std::string autoIndex) {
  _autoIndex = (0 == autoIndex.compare("on"));
}

/* ************************************************************************** */