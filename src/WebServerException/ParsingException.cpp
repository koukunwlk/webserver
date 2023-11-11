#include "WebServerException/ParsingException.hpp"

ParsingException::ParsingException() {
  this->message = "A parsing exception occured";
}

ParsingException::~ParsingException() throw () {}

ParsingException::ParsingException(std::string message) {
  this->message = message;
}

const char *ParsingException::what() const throw() {
  return this->message.data();
}
