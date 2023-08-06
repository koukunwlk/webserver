#pragma once

#include <iostream>
#include <string>

struct Header {
  // Start-Line (status)
  std::string protocolVersion;
  int statusCode;
  std::string reasonPhrase;

  // General-Header Fields
  std::string date;
  std::string transferEncoding;

  // Response-Header Fields
  std::string acceptRanges;
  

};

class Response {
 public:
  // Constructors
  Response();
  Response(const Response &copy);

  // Destructor
  ~Response();

  // Operators
  Response &operator=(const Response &assign);

  // Methods
  void setStatusCode(int statusCode);
  void setProtocolVersion(std::string protocol);
  void setReasonPhrase(std::string reasonPhrase);
  void setDate(std::string date);
  void setTransferEncoding(std::string transferEncoding);

  // Accessors
  int getStatusCode() const;
  std::string getProtocolVersion() const;
  std::string getReasonPhrase() const;
  std::string getDate() const;
  std::string getTransferEncoding() const;

 private:
  Header _header;
};
