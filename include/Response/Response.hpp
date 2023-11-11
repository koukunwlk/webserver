#pragma once

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#define HTTP_OK 200
#define HTTP_CREATED 201
#define HTTP_ACCEPTED 202
#define HTTP_NO_CONTENT 204
#define HTTP_MOVED 301
#define HTTP_BAD_REQUEST 400
#define HTTP_UNAUTHORIZED 401
#define HTTP_FORBIDDEN 403
#define HTTP_NOT_FOUND 404
#define HTTP_NOT_ALLOWED 405
#define HTTP_REQUEST_TIME_OUT 408
#define HTTP_LENGTH_REQUIRED 411
#define HTTP_UNSUPPORTED_MEDIA_TYPE 415
#define HTTP_TOO_MANY_REQUESTS 429
#define HTTP_INTERNAL_SERVER_ERROR 500
#define HTTP_NOT_IMPLEMENTED 501
#define HTTP_BAD_GATEWAY 502
#define HTTP_SERVICE_UNAVAILABLE 503

typedef struct ResponseHeader {
  // Start-Line (status)
  std::string protocolVersion;
  int statusCode;
  std::string reasonPhrase;

  // Entity-Header Fields
  int contentLength;
  std::string contentType;

  std::string location;
} resHeader_t;

class Response {
 public:
  // Constructors
  Response();
  Response(int statusCode, std::string contentType, const std::string body);
  Response(const Response &copy);

  // Destructor
  ~Response();

  // Operators
  Response &operator=(const Response &assign);
  friend std::ostream &operator<<(std::ostream &os, const Response &res);

  // Accessors
  void setProtocolVersion(std::string protocol);
  void setStatusCode(int statusCode);
  void setReasonPhrase(int setReasonPhrase);
  void setReasonPhrase(std::string reasonPhrase);
  void setContentLength(int contentLength);
  void setContentType(std::string contentType);
  void setBody(std::string body);
  std::string getProtocolVersion() const;
  int getStatusCode() const;
  std::string getReasonPhrase() const;
  int getContentLength() const;
  std::string getContentType() const;
  std::string getBody() const;

  void setLocation(std::string);
  std::string getLocation() const;

 private:
  resHeader_t _header;
  std::string _body;
};
