#pragma once

#include <exception>
#include <string>

class WebServerException : public std::exception {
 public:
  WebServerException();
  virtual ~WebServerException() throw ();
  virtual const char *what() const throw();

 private:
  std::string message;
};

#include "WebServerException/ConfigException.hpp"
#include "WebServerException/HostingException.hpp"
#include "WebServerException/ParsingException.hpp"
#include "WebServerException/RequestValidationException.hpp"
