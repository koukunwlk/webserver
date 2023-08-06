#pragma once

#include <iostream>
#include <string>

struct Header {
  std::string protocolVersion;  //
  // Start-Line (status)
  int statusCode;
  std::string reasonPhrase;

  // Header Fields
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

  // Accessors
  int getStatusCode() const;
  std::string getProtocolVersion() const;
  std::string getReasonPhrase() const;

 private:
  Header _header;
};
