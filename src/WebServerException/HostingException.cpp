#include "WebServerException/HostingException.hpp"

HostingException::HostingException() {
  this->message = "A hosting exception occured";
}

HostingException::~HostingException() throw() {}

const char *HostingException::what() const throw() {
  return this->message.data();
}
