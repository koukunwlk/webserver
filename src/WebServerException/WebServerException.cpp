#include "WebServerException.hpp"

// WebServerException Exceptions ------------------------------

WebServerException::WebServerException() {
  this->message = "A WebServer exception occured";
}

const char *WebServerException::what() const throw() {
  return this->message.data();
}

// Parsing Exceptions ------------------------------

ParsingException::ParsingException() {
  this->message = "A parsing exception occured";
}

const char *ParsingException::what() const throw() {
  return this->message.data();
}

// Config Exceptions ------------------------------

ConfigException::ConfigException() {
  this->message = "A config exception occured";
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

// Hosting Exceptions ------------------------------

HostingException::HostingException() {
  this->message = "A hosting exception occured";
}

const char *HostingException::what() const throw() {
  return this->message.data();
}