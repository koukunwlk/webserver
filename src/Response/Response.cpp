#include "Response/Response.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response() {}

Response::Response(int statusCode, std::string contentType,
                   const std::string &body) {
  _header.protocolVersion = "HTTP/1.1";
  _header.statusCode = statusCode;
  setReasonPhrase(statusCode);
  _header.contentType = contentType;
  _header.contentLenght = body.length();
  _body = body;
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
  if (statusCode == 200) _header.reasonPhrase = "OK";
  if (statusCode == 404) _header.reasonPhrase = "Not Found";
  if (statusCode == 500) _header.reasonPhrase = "Internal Server Error";
}

void Response::setContentType(std::string contentType) {
  _header.contentType = contentType;
}

void Response::setContentLength(int contentLenght) {
  _header.contentLenght = contentLenght;
}

void Response::setBody(std::string body) { _body = body; }

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int Response::getStatusCode() const { return _header.statusCode; }

std::string Response::getProtocolVersion() const {
  return _header.protocolVersion;
}

std::string Response::getReasonPhrase() const { return _header.reasonPhrase; }

std::string Response::getContentType() const { return _header.contentType; }

int Response::getContentLength() const { return _header.contentLenght; }

std::string Response::getBody() const { return _body; }
