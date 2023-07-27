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

class Request {
 public:
  Request(const char *);
  ~Request();

  std::string getRawData() const;

  std::string getBody() const;

  std::string getHeaderMethod() const;
  std::string getHeaderTarget() const;
  std::string getHeaderProtocol() const;
  std::string getHeaderHost() const;
  std::string getHeaderContentType() const;
  int getHeaderContentLength() const;

 private:
  std::string _rawData;
  header_t _header;
  std::string _body;

  void parseRequestData();
  void parseRawHeader();
  void parseHeaderProperties();
  void parseBody();
  void parseRequestLine(std::string line);
  void parseGeneralHeader(std::string line);
  std::string getPropertyValueFrom(std::string line);

  void validate();
  void validateMethod();
  void validateContentLength();
  void validateBody();
};

std::ostream &operator<<(std::ostream &o, Request const &i);
