#pragma once

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include "WebServerException/WebServerException.hpp"

#define CRLF "\r\n\r\n"

typedef struct RequestHeader {
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

} reqHeader_t;

class Request {
 public:
  Request(const char *);
  ~Request();

  std::string getRawData() const;

  char *getBody() const;

  std::string getHeaderRawDate() const;
  std::string getHeaderMethod() const;
  std::string getHeaderTarget() const;
  std::string getHeaderProtocol() const;
  std::string getHeaderHost() const;
  std::string getHeaderContentType() const;
  int getHeaderContentLength() const;

  int getValidationStatus() const;
  int setValidationStatus(int);

  std::string getServerRoot() const;
  void setServerRoot(std::string);
  std::string getRedirect() const;
  void setRedirect(std::string);
  std::string getIndex() const;
  void setIndex(std::string);
  bool getAutoIndex() const;
  void setAutoIndex(std::string);

 private:
  const char *_rawData;
  reqHeader_t _header;
  char *_body;

  // Propriedades do ServerConfig
  std::string _serverRoot;
  std::string _redirect;
  std::string _index;
  bool _autoIndex;

  int _validationStatus;

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
