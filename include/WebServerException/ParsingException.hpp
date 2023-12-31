#pragma once

#include <string>

#include "WebServerException/WebServerException.hpp"

class ParsingException : public WebServerException {
 public:
  ParsingException();
  ParsingException(std::string message);
  virtual ~ParsingException() throw();

  virtual const char *what() const throw();

 private:
  std::string message;
};
