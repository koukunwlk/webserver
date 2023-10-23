#pragma once

#include "Response/Response.hpp"
#include "Request/Request.hpp"
#include "WebServerException/WebServerException.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <cstring>
#include <dirent.h>

#define VALID_REQUEST 0
#define INVALID_HEADER  1
#define INVALID_METHOD 2


class Handler {
 public:
  Handler();
  Handler(Request*);
  ~Handler();

  Response &getResponse();

  int handleRequest();
  int handleError();

 private:
  Request *_req;
  Response _res;
};

std::string getHtmlPage(std::string root, std::string filepath);
std::string getPhpPage(std::string root, std::string filepath);
std::string listFolder(std::string fullpath);
bool endsWith(std::string fullString, std::string ending);
