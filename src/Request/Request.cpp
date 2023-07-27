#include "Request/Request.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request(const char* rawData) : _rawData(rawData) {
  parseRequestData();
  validate();
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
  parseBody();
}

void Request::parseRawHeader() {
  std::size_t delim = _rawData.find(CRLF);
  if (delim == std::string::npos) {
    throw RequestValidationException();
  }

  _header.rawData = _rawData.substr(0, delim);
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
    throw RequestValidationException();
  }
  std::string propertyValue = line.substr(colon + 1);

  while (propertyValue[0] == ' ') {
    propertyValue = propertyValue.substr(1);
  }

  return propertyValue;
}

void Request::parseBody() { _body = _rawData.substr(_rawData.find(CRLF) + 4); }

void Request::validate() {
  validateMethod();
  validateContentLength();
  validateBody();
}

void Request::validateMethod() {
  std::string acceptedMethods[] = {"GET", "POST", "DELETE"};
  bool validatedMethod = FALSE;

  for (int i = 0; i < 3; i++) {
    if (_header.method.find(acceptedMethods[i]) != std::string::npos) {
      validatedMethod = TRUE;
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
  size_t contentLength = static_cast<size_t>(_header.contentLength);
  if (contentLength != _body.size()) {
    throw RequestValidationException();
  }
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string Request::getBody() const { return _body; }

std::string Request::getRawData() const { return _rawData; }

std::string Request::getHeaderTarget() const { return _header.target; }

std::string Request::getHeaderMethod() const { return _header.method; }

std::string Request::getHeaderProtocol() const { return _header.protocol; }

std::string Request::getHeaderHost() const { return _header.host; }

std::string Request::getHeaderContentType() const {
  return _header.contentType;
}

int Request::getHeaderContentLength() const { return _header.contentLength; }

/* ************************************************************************** */