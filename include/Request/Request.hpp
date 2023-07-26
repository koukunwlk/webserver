#pragma once

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "WebServerException/WebServerException.hpp"

#define TRUE 1
#define FALSE 0

#define CRLF "\r\n\r\n"

typedef struct Header {
  std::string rawData;

  // Request-Line
  std::string method;
  std::string target;
  std::string protocol;

  // Request Header Fields
  std::string host;

  // Entity Header Fields
  std::string contentType;
  int contentLength;

  // Hop-by-hop headers
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

header_t getHeader(std::string rawData);
void getRequestLine(header_t &header, std::string line);
void getGeneralHeader(header_t &header, std::string line);
void validateHeader(header_t &header);
void validateBodyLength(size_t contentLength, size_t bodySize);
