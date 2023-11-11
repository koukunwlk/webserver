#include "WebServerException/WebServerException.hpp"

WebServerException::WebServerException() {
  this->message = "A WebServer exception occured";
}

WebServerException::~WebServerException() throw() {}

const char *WebServerException::what() const throw() {
  return this->message.data();
}
