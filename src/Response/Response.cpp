#include "Response/Response.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response() { _header.protocolVersion = "HTTP/1.1"; }

Response::Response(int statusCode, std::string contentType,
                   const std::string body) {
  _header.protocolVersion = "HTTP/1.1";
  _header.statusCode = statusCode;
  setReasonPhrase(statusCode);
  _header.contentType = contentType;
  _header.contentLength = body.length();
  _body = body;
}

Response::Response(const Response &copy) {
  *this = copy;
}
/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Response::~Response() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Response &Response::operator=(const Response &assign) {
  if (this != &assign) {
    _header = assign._header;
    _body = assign._body;
  }
  return *this;
}

std::ostream &operator<<(std::ostream &os, const Response &res) {
  os << res.getProtocolVersion() << " " << res.getStatusCode() << " "
     << res.getReasonPhrase() << "\r\n"
     << "Content-Type: " << res.getContentType() << "\r\n"
     << "Content-Length: " << res.getContentLength() << "\r\n"
     << "\r\n"
     << res.getBody();
  return os;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int Response::getStatusCode() const { return _header.statusCode; }

std::string Response::getProtocolVersion() const {
  return _header.protocolVersion;
}

std::string Response::getReasonPhrase() const { return _header.reasonPhrase; }

std::string Response::getContentType() const { return _header.contentType; }

int Response::getContentLength() const { return _header.contentLength; }

std::string Response::getBody() const { return _body; }

void Response::setStatusCode(int statusCode) {
  _header.statusCode = statusCode;
}

void Response::setProtocolVersion(std::string protocol) {
  _header.protocolVersion = protocol;
}

void Response::setReasonPhrase(std::string reasonPhrase) {
  _header.reasonPhrase = reasonPhrase;
}

void Response::setReasonPhrase(int statusCode) {
  std::string reasonPhrase;

  switch (statusCode) {
    case 200:
      reasonPhrase = "OK";
      break;
    case 404:
      reasonPhrase = "Not Found";
      break;
    case 500:
      reasonPhrase = "Internal Server Error";
      break;
    default:
      reasonPhrase = "Unknown";
      break;
  }
  _header.reasonPhrase = reasonPhrase;
}

void Response::setContentType(std::string contentType) {
  _header.contentType = contentType;
}

void Response::setContentLength(int contentLength) {
  _header.contentLength = contentLength;
}

void Response::setBody(std::string body) {
  _body = body;
  _header.contentLength = body.length();
}

void Response::setLocation(std::string redirect) {
  _header.location = redirect;
  this->setStatusCode(301);
  this->setReasonPhrase("Moved Permanently");
}
