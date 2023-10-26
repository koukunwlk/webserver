#pragma once

#include <dirent.h>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <iostream>

#include "Request/Request.hpp"
#include "Response/Response.hpp"
#include "WebServerException/WebServerException.hpp"

#define VALID_REQUEST 0
#define INVALID_HEADER 1
#define INVALID_METHOD 2

class Handler {
 public:
  Handler();
  Handler(Request *);
  ~Handler();

  Response &getResponse();

  void setErrorPage(int code);
  void handleRequest();
  int handleGET();
  // int handlePOST();
  // int handleDELETE();
  int handleError();

 private:
  Request *_req;
  Response _res;
};

int getHtmlPage(std::string &page, std::string root, std::string filepath);
int getPhpPage(std::string &page, std::string root, std::string filepath);
int getFolder(Request &req, std::string &page, std::string root,
              std::string filepath);
int listFolderContent(std::string &page, std::string fullpath);
bool endsWith(std::string fullString, std::string ending);
