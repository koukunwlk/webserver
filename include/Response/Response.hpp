#pragma once

#include <iostream>
#include <string>

struct Header {
  // Start-Line (status)
  std::string protocolVersion;
  int statusCode;
  std::string reasonPhrase;

  // Entity-Header Fields
  int contentLenght;
  std::string contentType;
};

class Response {
 public:
  // Constructors
  Response();
  Response(int statusCode, std::string contentType, const std::string &body);
  Response(const Response &copy);

  // Destructor
  ~Response();

  // Operators
  Response &operator=(const Response &assign);

  // Methods
  void setProtocolVersion(std::string protocol);
  void setStatusCode(int statusCode);
  void setReasonPhrase(int setReasonPhrase);
  void setReasonPhrase(std::string reasonPhrase);
  void setContentLength(int contentLenght);
  void setContentType(std::string contentType);
  void setBody(std::string body);

  // Accessors
  std::string getProtocolVersion() const;
  int getStatusCode() const;
  std::string getReasonPhrase() const;
  int getContentLength() const;
  std::string getContentType() const;
  std::string getBody() const;

 private:
  Header _header;
  std::string _body;
};
