#include "Response/Response.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response() {}

Response::Response(const Response &copy) {
  if (this != &copy) *this = copy;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Response::~Response() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Response &Response::operator=(const Response &assign) {
  if (this != &assign) _header = assign._header;
  return *this;
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

void Response::setDate(std::string date) { _header.date = date; }

void Response::setTransferEncoding(std::string transferEncoding) {
  _header.transferEncoding = transferEncoding;
}

void Response::setAcceptRanges(std::string acceptRanges) {
  _header.acceptRanges = acceptRanges;
}

void Response::setLocation(std::string location) {
  _header.location = location;
}

void Response::setServer(std::string server) { _header.server = server; }

void Response::setWwwAuthenticate(std::string wwwAuthenticate) {
  _header.wwwAuthenticate = wwwAuthenticate;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int Response::getStatusCode() const { return _header.statusCode; }

std::string Response::getProtocolVersion() const {
  return _header.protocolVersion;
}

std::string Response::getReasonPhrase() const { return _header.reasonPhrase; }

std::string Response::getDate() const { return _header.date; }

std::string Response::getTransferEncoding() const {
  return _header.transferEncoding;
}

std::string Response::getAcceptRanges() const { return _header.acceptRanges; }

std::string Response::getLocation() const { return _header.location; }

std::string Response::getServer() const { return _header.server; }

std::string Response::getWwwAuthenticate() const {
  return _header.wwwAuthenticate;
}
