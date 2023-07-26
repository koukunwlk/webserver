#pragma once

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "WebServerException/WebServerException.hpp"

#define TRUE 1
#define FALSE 0

typedef struct Header {
  std::string rawData;
  std::string method;
  std::string target;
  std::string protocol;
  std::string host;
  std::string contentType;
  int contentLength;
  bool keepAlive;
} header_t;

typedef enum { method, target, host, contentType, contentLength } prop_t;

class Request {
 public:
  std::string rawData;
  header_t header;
  std::string body;
  Request(const char *);
  ~Request();

 private:
};

void getRequestLine(header_t &header, std::string line);
void getGeneralHeader(header_t &header, std::string line);
void validateHeader(header_t &header);

header_t getHeader(std::string rawData);