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
    virtual ~InvalidMethod() throw();
    virtual const char *what() const throw();
  };

  class InvalidFormat : public WebServerException {
   private:
    std::string _message;

   public:
    InvalidFormat();
    virtual ~InvalidFormat() throw();
    virtual const char *what() const throw();
  };
  RequestValidationException();
  virtual ~RequestValidationException() throw();
  virtual const char *what() const throw();

 private:
  std::string _message;
};
