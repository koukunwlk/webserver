#include "WebServerException/ParsingException.hpp"

ParsingException::ParsingException() {
  this->message = "A parsing exception occured";
}

const char *ParsingException::what() const throw() {
  return this->message.data();
}
