#pragma once

#include <stdexcept>
#include <string>

class WebServerException : public std::exception {
 public:
  WebServerException();
  virtual const char *what() const throw();

 private:
  std::string message;
};

class ParsingException : public WebServerException {
 public:
  ParsingException();
  virtual const char *what() const throw();

 private:
  std::string message;
};

class ConfigException : public WebServerException {
 public:
  class FileNotFound : public std::exception {
   private:
    std::string message;

   public:
    FileNotFound(const char *);
    virtual const char *what() const throw();
  };
  ConfigException();
  virtual const char *what() const throw();

 private:
  std::string message;
};

class HostingException : public WebServerException {
 public:
  HostingException();
  virtual const char *what() const throw();

 private:
  std::string message;
};