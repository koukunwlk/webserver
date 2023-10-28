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

void Handler::setErrorPage(int code) {
  _res.setStatusCode(code);
  switch (code) {
    case HTTP_BAD_REQUEST:
      _res.setReasonPhrase("Bad Request");
      break;
    case HTTP_FORBIDDEN:
      _res.setReasonPhrase("Forbidden");
      break;
    case HTTP_NOT_FOUND:
      _res.setReasonPhrase("Not Found");
      break;
    default:
      _res.setReasonPhrase("Server Internal Error");
      _res.setStatusCode(500);
      break;
  }
  // TODO: pegar errorfile do server que respondeu a request
  std::stringstream ss;
  ss << "/" << code << ".html";
  std::string errorFile = ss.str();
  std::string page;
  getHtmlPage(page, "www", errorFile);
  _res.setBody(page);
}

int Handler::handleError() {
  setErrorPage(HTTP_BAD_REQUEST);
  return 0;
}

int Handler::handleGET() {
  std::string page;
  std::string redirect = _req->getRedirect();
  std::string rootFolder = _req->getServerRoot();
  std::string filePath = _req->getHeaderTarget();
  int status = 0;

  if (!redirect.empty()) {
    _res.setLocation(redirect);
    return 0;
  }

  if (endsWith(filePath, ".php")) {
    status += getPhpPage(page, rootFolder, filePath);
  } else if (endsWith(filePath, ".html")) {
    status += getHtmlPage(page, rootFolder, filePath);
  } else if (endsWith(filePath, "/") ||
             filePath.find(".") == std::string::npos) {
    status += getFolder(*_req, page, rootFolder, filePath);
  }

  if (status == 0) {
    _res.setBody(page);
    _res.setStatusCode(200);
    _res.setReasonPhrase("OK");
  } else {
    setErrorPage(HTTP_NOT_FOUND);
  }
  return 0;
}

int Handler::handlePOST() {
  _res.setStatusCode(201);
  _res.setReasonPhrase("CREATED");
  return 0;
}

int Handler::handleDELETE() {
  std::string rootFolder = _req->getServerRoot();
  std::string fileName = _req->getBody();
  std::string fullPath = rootFolder + "/" + fileName;

  if (access(fullPath.c_str(), F_OK)) {
    std::cout << fullPath << "\n";
    return 1;
  }

  remove(fullPath.c_str());
  _res.setStatusCode(200);
  _res.setReasonPhrase("OK");
  return 0;
}

void Handler::handleRequest() {
  int status = 0;

  if (_req->getHeaderMethod().compare("GET") == 0) {
    status += handleGET();
  } else if (_req->getHeaderMethod().compare("POST") == 0) {
    status += handlePOST();
  } else if (_req->getHeaderMethod().compare("DELETE") == 0) {
    status += handleDELETE();
  }

  if (status != 0) {
    return;
  }
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

Response &Handler::getResponse() { return this->_res; }

/*
** --------------------------------- STATICS ---------------------------------
*/

int getHtmlPage(std::string &page, std::string root, std::string filepath) {
  std::fstream fileStream;
  std::string tmp;
  std::string fullPath = root + filepath;

  if (access(fullPath.c_str(), F_OK)) {
    return 1;
  }

  fileStream.open(fullPath.c_str(), std::ios_base::in);
  while (std::getline(fileStream, tmp)) {
    page += tmp;
  }

  return 0;
}

int getPhpPage(std::string &page, std::string root, std::string filepath) {
  std::stringstream ss;
  std::string fullPath = root + filepath;

  if (access(fullPath.c_str(), F_OK)) {
    return 1;
  }

  int fd[2];
  pipe(fd);
  int pid = fork();

  if (pid == 0) {
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);

    char cgi_path[] = "/usr/bin/php-cgi";
    std::string relativePath = "." + filepath;
    char *const command[] = {cgi_path, (char *)relativePath.c_str(), NULL};

    chdir(root.c_str());
    execve(cgi_path, command, NULL);
  }

  close(fd[1]);
  char buffer[512];
  while (read(fd[0], &buffer, sizeof(buffer - 1))) {
    ss << buffer;
    memset(buffer, 0, 512);
  }
  close(fd[0]);
  page = ss.str();

  page = page.substr(page.find("<"), page.length());
  return 0;
}

int getFolder(Request &req, std::string &page, std::string rootFolder,
              std::string filePath) {
  std::string fullpath = rootFolder + filePath + req.getIndex();

  if (access(fullpath.c_str(), F_OK) == 0) {
    return getHtmlPage(page, rootFolder + filePath, req.getIndex());
  }

  if (req.getAutoIndex() == true) {
    return listFolderContent(page, rootFolder + filePath);
  }

  return 1;
}

int listFolderContent(std::string &page, std::string fullpath) {
  DIR *dir = opendir(fullpath.c_str());
  dirent *dirent = readdir(dir);
  while (dirent) {
    std::string temp = dirent->d_name;
    if (temp.compare(".") && temp.compare("..")) {
      page += dirent->d_name;
      page += '\n';
    }
    dirent = readdir(dir);
  }
  return 0;
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