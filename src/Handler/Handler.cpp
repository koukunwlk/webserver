#include "Handler/Handler.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Handler::Handler(Request &req) : _req(&req) {
  if (_req->getValidationStatus() == VALID_REQUEST) {
    this->handleRequest();
  } else {
    this->handleError(_req->getValidationStatus());
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
  _res.setReasonPhrase(getPhrase(code));

  std::vector<ErrorPage> errorPages = _req->getErrorPages();
  std::string rootFolder = _req->getServerRoot();
  std::string page;

  std::stringstream ss;
  ss << "/" << code << ".html";
  std::string errorFile = ss.str();
  getHtmlPage(page, "", "www", errorFile);

  for (int i = 0; i < (int)errorPages.size(); i++) {
    if (errorPages[i].code == code) {
      page.clear();
      getHtmlPage(page, "", rootFolder, errorPages[i].path);
    }
  }
  _res.setBody(page);
}

int Handler::handleError(int status) {
  switch (status) {
    case INVALID_HEADER:
      setErrorPage(403);
      break;
    case INVALID_LOCATION:
      setErrorPage(404);
      break;
    case INVALID_METHOD:
      setErrorPage(405);
      break;
    default:
      setErrorPage(500);
      break;
  }
  return 0;
}

int Handler::handleGET() {
  std::string page;
  std::string redirect = _req->getRedirect();
  std::string rootFolder = _req->getServerRoot();
  std::string filePath = _req->getHeaderTarget();
  std::string location = _req->getServerLocationUrl();
  int status = 0;

  if (!redirect.empty()) {
    _res.setLocation(redirect);
    return 0;
  }

  if (endsWith(filePath, ".php")) {
    status += getPhpPage(page, location, rootFolder, filePath);
  } else if (endsWith(filePath, ".html")) {
    status += getHtmlPage(page, location, rootFolder, filePath);
  } else if (endsWith(filePath, "/") ||
             filePath.find(".") == std::string::npos) {
    status += getFolder(*_req, page, location, rootFolder, filePath);
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
  std::string contentType = _req->getHeaderContentType();
  std::string requestRawData(_req->getCharRawData());
  std::vector<unsigned char> rawData = _req->getRawData();

  std::string boundary = contentType.substr(contentType.find("boundary=") + 9);
  size_t filenameInitPos = requestRawData.find("filename=") + 10;
  size_t filenameEndPos = requestRawData.find("\"", filenameInitPos);
  std::string filename =
      requestRawData.substr(filenameInitPos, filenameEndPos - filenameInitPos);
  size_t nextCrlfPos = requestRawData.find(CRLF, filenameEndPos) + 4;
  size_t closingBoundary = rawData.size() - boundary.length() - 7;

  std::ofstream uploadedFile;
  uploadedFile.open(filename.c_str(), std::ios::out | std::ios::binary);

  uploadedFile.write((char *)&rawData[nextCrlfPos],
                     closingBoundary - nextCrlfPos);

  uploadedFile.close();

  _res.setStatusCode(201);
  _res.setReasonPhrase("CREATED");
  return 0;
}

int Handler::handleDELETE() {
  std::string rootFolder = _req->getServerRoot();
  std::string fileName = _req->getBody();
  std::string fullPath = rootFolder + "/" + fileName;

  if (access(fullPath.c_str(), F_OK)) {
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

int getHtmlPage(std::string &page, std::string location, std::string root,
                std::string filepath) {
  std::string fileParsed = filepath.substr(location.length());
  if (fileParsed[0] != '/') {
    fileParsed.insert(0, 1, '/');
  }
  std::fstream fileStream;
  std::string tmp;
  std::string fullPath = root + fileParsed;

  if (access(fullPath.c_str(), F_OK)) {
    return 1;
  }

  fileStream.open(fullPath.c_str(), std::ios_base::in);
  while (std::getline(fileStream, tmp)) {
    page += tmp;
  }

  return 0;
}

int getPhpPage(std::string &page, std::string location, std::string root,
               std::string filepath) {
  std::stringstream ss;
  std::string fileParsed = filepath.substr(location.length());
  if (fileParsed[0] != '/') {
    fileParsed.insert(0, 1, '/');
  }
  std::string fullPath = root + fileParsed;

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
    std::string relativePath = "." + fileParsed;
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

int getFolder(Request &req, std::string &page, std::string location,
              std::string rootFolder, std::string filepath) {
  std::string fileParsed = filepath.substr(location.length());
  if (fileParsed.length() == 0) {
    fileParsed = '/';
  }
  if (fileParsed[0] != '/') {
    fileParsed.insert(0, 1, '/');
  }
  std::string fullpath;
  std::vector<std::string> indexes = req.getIndex();

  for (int i = 0; i < (int)indexes.size(); i++) {
    fullpath = rootFolder + fileParsed + "/" + indexes[i];
    if (access(fullpath.c_str(), F_OK) == 0) {
      return getHtmlPage(page, "", rootFolder + fileParsed, "/" + indexes[i]);
    }
  }

  if (req.getAutoIndex() == true) {
    return listFolderContent(page, rootFolder + fileParsed);
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

std::string getPhrase(int code) {
  std::string phrase;
  switch (code) {
    case HTTP_BAD_REQUEST:
      phrase = "Bad Request";
      break;
    case HTTP_FORBIDDEN:
      phrase = "Forbidden";
      break;
    case HTTP_NOT_FOUND:
    default:
      phrase = "Not Found";
      break;
  }
  return phrase;
}

/* ************************************************************************** */
