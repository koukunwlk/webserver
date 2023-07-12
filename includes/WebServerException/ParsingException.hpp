#pragma once

#include <string>

#include "WebServerException/WebServerException.hpp"

class ParsingException : public WebServerException {
 public:
  ParsingException();
  virtual const char *what() const throw();

 private:
  std::string message;
};
