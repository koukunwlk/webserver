#include "WebServerException/RequestValidationException.hpp"

RequestValidationException::RequestValidationException() {
  this->message = "A Request Validation exception occured";
}

const char *RequestValidationException::what() const throw() {
  return this->message.data();
}

RequestValidationException::InvalidMethod::InvalidMethod(const char *method) {
  this->message += "Invalid Method: ";
  this->message += method;
}

const char *RequestValidationException::InvalidMethod::what() const throw() {
  return this->message.data();
}