#include "WebServerException/ConfigException.hpp"

ConfigException::ConfigException() {
  this->message = "A config exception o cured";
}

const char *ConfigException::what() const throw() {
  return this->message.data();
}

ConfigException::FileNotFound::FileNotFound(const char *filename) {
  this->message += "File not found: ";
  this->message += filename;
}

const char *ConfigException::FileNotFound::what() const throw() {
  return this->message.data();
}
