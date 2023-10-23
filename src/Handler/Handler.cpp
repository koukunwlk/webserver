#include "Handler/Handler.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Handler::Handler(Request *req) : _req(req) {
  if (_req->getValidationStatus() == VALID_REQUEST) {
    this->handleRequest();
  } else {
    this->handleError();
  }
}

Handler::Handler() {}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Handler::~Handler() {}

/*
** --------------------------------- METHODS ----------------------------------
*/

int Handler::handleError() {
  _res.setStatusCode(HTTP_BAD_REQUEST);
  _res.setReasonPhrase("Bad Request");
  std::string page = getHtmlPage("www", "/400.html");
  _res.setBody(page);
  return 0;
}

int Handler::handleRequest() {
  std::string page;
  std::string redirect = _req->getRedirect();
  if (!redirect.empty()) {
    _res.setLocation(redirect);
    return 0;
  }
  std::string rootFolder = _req->getServerRoot();
  std::string filePath = _req->getHeaderTarget();
  if (endsWith(filePath, ".php")) {
    page = getPhpPage(rootFolder, filePath);
  } else if (endsWith(filePath, ".html")) {
    page = getHtmlPage(rootFolder, filePath);
  } else if (endsWith(filePath, "/")) {
    std::string fullpath = rootFolder + filePath + _req->getIndex();
    if (access(fullpath.c_str(), F_OK)) {
      if (_req->getAutoIndex() == true) {
        page = listFolder(rootFolder + filePath);
      } else {
        page = getHtmlPage("www", "/404.html");
        _res.setStatusCode(HTTP_NOT_FOUND);
        _res.setReasonPhrase("Not Found");
        _res.setBody(page);
        return 0;
      }
    } else {
      page = getHtmlPage(rootFolder + filePath, _req->getIndex());
    }
  }
  _res.setBody(page);
  _res.setStatusCode(200);
  _res.setReasonPhrase("OK");
  return 0;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

Response &Handler::getResponse() { return this->_res; }

/*
** --------------------------------- STATICS ---------------------------------
*/

std::string getHtmlPage(std::string root, std::string filepath) {
  std::fstream fileStream;
  std::string tmp;
  std::string response;
  std::string fullpath = root + filepath;
  fileStream.open(fullpath.c_str(), std::ios_base::in);
  while (std::getline(fileStream, tmp)) {
    response += tmp;
  }
  return response;
}

std::string getPhpPage(std::string root, std::string filepath) {
  int fd[2];
  int pid;
  char buffer[1024];
  memset(buffer, 0, 1024);

  char cgi_path[] = "/usr/bin/php-cgi";
  std::string relativePath = "." + filepath;
  char *const command[] = {cgi_path, (char *)relativePath.c_str(), NULL};

  pipe(fd);
  pid = fork();
  if (pid == 0) {
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    chdir(root.c_str());
    execve(cgi_path, command, NULL);
  }
  close(fd[1]);
  read(fd[0], &buffer, sizeof(buffer));
  close(fd[0]);
  std::string page(buffer);

  return page.substr(page.find('\r') + 4, page.length());
}

std::string listFolder(std::string fullpath) {
  dirent *dirent;
  DIR *dir = opendir(fullpath.c_str());
  std::string temp;
  std::string dirList;

  dirent = readdir(dir);
  while (dirent) {
    temp = dirent->d_name;
    if (temp.compare(".") && temp.compare("..")) {
      dirList += dirent->d_name;
      dirList += '\n';
    }
    dirent = readdir(dir);
  }

  return dirList;
}

bool endsWith(std::string fullString, std::string ending) {
  if (fullString.length() >= ending.length()) {
    return (0 == fullString.compare(fullString.length() - ending.length(),
                                    ending.length(), ending));
  } else {
    return false;
  }
}

/* ************************************************************************** */