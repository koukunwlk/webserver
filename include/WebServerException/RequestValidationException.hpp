#pragma once

#include <string>

#include "WebServerException/WebServerException.hpp"

#define DEFAULT_MESSAGE "A Request Validation exception occured"

class RequestValidationException : public WebServerException {
 public:
  class InvalidMethod : public WebServerException {
   private:
    std::string _message;

   public:
    InvalidMethod(const char *);
    virtual const char *what() const throw();
  };

  class InvalidFormat : public WebServerException {
   private:
    std::string _message;

   public:
    InvalidFormat();
    virtual const char *what() const throw();
  };
  RequestValidationException();
  virtual const char *what() const throw();

 private:
  std::string _message;
};
