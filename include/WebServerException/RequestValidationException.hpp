#pragma once

#include <string>

#include "WebServerException/WebServerException.hpp"

class RequestValidationException : public WebServerException {
 public:
  class InvalidMethod : public WebServerException {
   private:
    std::string message;

   public:
    InvalidMethod(const char *);
    virtual const char *what() const throw();
  };
  RequestValidationException();
  virtual const char *what() const throw();

 private:
  std::string message;
};
