#include "WebServerException/ConfigException.hpp"

ConfigException::ConfigException() {
  this->message = "A config exception occured";
}

ConfigException::~ConfigException() throw(){}

const char *ConfigException::what() const throw() {
  return this->message.data();
}

ConfigException::FileNotFound::FileNotFound(const char *filename) {
  this->message += "File not found: ";
  this->message += filename;
}

ConfigException::FileNotFound::~FileNotFound() throw() {}

const char *ConfigException::FileNotFound::what() const throw() {
  return this->message.data();
}
