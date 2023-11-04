#pragma once

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ConfigFileParser/ConfigFileParser.hpp"
#include "WebServerException/WebServerException.hpp"
#include "webserver.hpp"

#define CRLF "\r\n\r\n"
#define VALID_REQUEST 0
#define INVALID_HEADER 1
#define INVALID_METHOD 2
#define INVALID_LOCATION 3

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
  Request(std::vector<unsigned char>, ServerConfig);
  Request(std::vector<unsigned char>);
  ~Request();

  char *getCharRawData();
  std::vector<unsigned char> getRawData();

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
  std::vector<std::string> getIndex() const;
  void setIndex(std::vector<std::string>);
  std::vector<ErrorPage> getErrorPages() const;
  void setErrorPages(std::vector<ErrorPage>);
  bool getAutoIndex() const;
  void setAutoIndex(std::string);
  std::string getUploadStore() const;
  void setUploadStore(std::string);

 private:
  std::vector<unsigned char> _rawData;
  reqHeader_t _header;
  char *_body;

  // Propriedades do ServerConfig
  std::string _root;
  std::string _redirect;
  std::vector<std::string> _index;
  std::vector<ErrorPage> _errorPages;
  bool _autoIndex;
  std::string _uploadStore;

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
bool isFile(std::string contentType);
