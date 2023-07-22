#pragma once

#include <string>

struct Header {
  std::string method;
  std::string target;
  std::string host;
  std::string contentType;
  int contentLength;
};

class Request {
 public:
 private:
  Header _header;
  std::string _body;
  bool _keepAlive;
};