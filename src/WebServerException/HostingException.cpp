#include "WebServerException/HostingException.hpp"

HostingException::HostingException() {
  this->message = "A hosting exception occured";
}

const char *HostingException::what() const throw() {
  return this->message.data();
}
