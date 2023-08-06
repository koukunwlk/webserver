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
  std::string location;
  std::string server;
  std::string wwwAuthenticate;
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
  void setAcceptRanges(std::string acceptRanges);
  void setLocation(std::string location);
  void setServer(std::string server);
  void setWwwAuthenticate(std::string wwwAuthenticate);

  // Accessors
  int getStatusCode() const;
  std::string getProtocolVersion() const;
  std::string getReasonPhrase() const;
  std::string getDate() const;
  std::string getTransferEncoding() const;
  std::string getAcceptRanges() const;
  std::string getLocation() const;
  std::string getServer() const;
  std::string getWwwAuthenticate() const;

 private:
  Header _header;
};
