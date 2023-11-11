#pragma once

#include <string>

#include "WebServerException/WebServerException.hpp"

class HostingException : public WebServerException {
 public:
  HostingException();
  virtual ~HostingException() throw();
  virtual const char *what() const throw();

 private:
  std::string message;
};
