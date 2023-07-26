#include "Request/Request.hpp"

Request::Request(const char* data) : rawData(data) {
  std::size_t delim = this->rawData.find("\r\n\r\n");

  this->header.rawData = this->rawData.substr(0, delim);

  this->header = getHeader(this->header.rawData);

  this->body = this->rawData.substr(delim + 2);
}

Request::~Request() {}

header_t getHeader(std::string rawData) {
  header_t header;

  std::istringstream iss(rawData);
  std::string line;

  std::getline(iss, line);
  getRequestLine(header, line);

  while (std::getline(iss, line)) {
    getGeneralHeader(header, line);
  }

  validateHeader(header);

  return header;
}

void validateHeader(header_t& header) {
  std::string acceptedMethods[] = {"GET", "POST", "DELETE"};
  bool validatedMethod = FALSE;

  for (int i = 0; i < 3; i++) {
    if (header.method.find(acceptedMethods[i]) != std::string::npos) {
      validatedMethod = TRUE;
      break;
    }
  }
  if (!validatedMethod) {
    throw RequestValidationException();
  }
}

void getRequestLine(header_t& header, std::string line) {
  std::istringstream lineStream(line);
  std::string token[3];

  for (int j = 0; j < 3; j++) {
    getline(lineStream, token[j], ' ');
  }
  header.method = token[0];
  header.target = token[1];
  header.protocol = token[2].substr(0, token[2].size() - 1);
}

void getGeneralHeader(header_t& header, std::string line) {
  std::string props[] = {"Host", "Content-Type", "Content-Length"};

  for (int i = 0; i < 4; i++) {
    if (line.find(props[i]) == std::string::npos) {
      continue;
    }
    std::string property = line.substr(props[i].size() + 2);
    switch (i) {
      case 0:
        header.host = property;
        break;
      case 1:
        header.contentType = property;
        break;
      case 2:
        header.contentLength = std::atoi(property.c_str());
        break;
      default:
        break;
    }
  }
}