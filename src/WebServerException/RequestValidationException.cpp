#include "WebServerException/RequestValidationException.hpp"

RequestValidationException::RequestValidationException()
    : _message(DEFAULT_MESSAGE) {}

RequestValidationException::~RequestValidationException() throw() {}

const char *RequestValidationException::what() const throw() {
  return _message.data();
}

RequestValidationException::InvalidMethod::InvalidMethod(const char *method)
    : _message(DEFAULT_MESSAGE) {
  _message += ": Invalid Method: ";
  _message += method;
}

RequestValidationException::InvalidMethod::~InvalidMethod() throw() {}

const char *RequestValidationException::InvalidMethod::what() const throw() {
  return _message.data();
}

RequestValidationException::InvalidFormat::InvalidFormat()
    : _message(DEFAULT_MESSAGE) {
  _message += ": Invalid Format";
}

RequestValidationException::InvalidFormat::~InvalidFormat() throw() {}

const char *RequestValidationException::InvalidFormat::what() const throw() {
  return _message.data();
}