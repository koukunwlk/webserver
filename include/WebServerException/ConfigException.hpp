#pragma once

#include <string>

#include "WebServerException/WebServerException.hpp"

class ConfigException : public WebServerException {
 public:
  class FileNotFound : public WebServerException {
   private:
    std::string message;

   public:
    FileNotFound(const char *);
    virtual ~FileNotFound() throw();
    virtual const char *what() const throw();
  };
  ConfigException();
  virtual ~ConfigException() throw();
  virtual const char *what() const throw();

 private:
  std::string message;
};
