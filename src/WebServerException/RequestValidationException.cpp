#include "WebServerException/RequestValidationException.hpp"

RequestValidationException::RequestValidationException()
    : _message(DEFAULT_MESSAGE) {}

const char *RequestValidationException::what() const throw() {
  return _message.data();
}

RequestValidationException::InvalidMethod::InvalidMethod(const char *method)
    : _message(DEFAULT_MESSAGE) {
  _message += ": Invalid Method: ";
  _message += method;
}

const char *RequestValidationException::InvalidMethod::what() const throw() {
  return _message.data();
}
RequestValidationException::InvalidFormat::InvalidFormat()
    : _message(DEFAULT_MESSAGE) {
  _message += ": Invalid Format";
}

const char *RequestValidationException::InvalidFormat::what() const throw() {
  return _message.data();
}