#include "WebServer/WebServer.hpp"

WebServer::WebServer() {
  FD_ZERO(&this->_readFds);
  FD_ZERO(&this->_writeFds);
}

WebServer::~WebServer() {}

int WebServer::cookSocket() {
  int optval = 1;
  _sockaddr.sin_family = AF_INET;
  _sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  _sockaddr.sin_port = htons(8000);

  int listenFd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  if (bind(listenFd, (const struct sockaddr*)&_sockaddr, sizeof(_sockaddr)))
    perror("bind");
  std::cout << "Listening on port: " << ntohs(_sockaddr.sin_port) << std::endl;

  if (listen(listenFd, 10)) perror("listen");

  if (fcntl(listenFd, F_SETFL, O_NONBLOCK)) perror("fcntl");

  FD_SET(listenFd, &_readFds);
  _listenSockets.push_back(listenFd);
  return 0;
}

void WebServer::raise() {
  // setup
  cookSocket();

  // run
  struct timeval timer;
  memset(&timer, 0, sizeof(timer));
  while (1) {
    timer.tv_sec = 1;
    timer.tv_usec = 0;
    fd_set readFds = _readFds;
    fd_set writeFds = _writeFds;

    if (select(FD_SETSIZE, &readFds, &writeFds, NULL, &timer) == -1) {
      perror("select: ");
    }

    // Handle
    for (std::vector<int>::iterator it = _listenSockets.begin();
         it != _listenSockets.end(); ++it) {
      int currentSockfd = *it;
      if (FD_ISSET(currentSockfd, &readFds)) {
        sockaddr_in client_addr;
        memset(&client_addr, 0, sizeof(client_addr));
        socklen_t addr_len = sizeof(client_addr);
        int clientSocket =
            accept(currentSockfd, (struct sockaddr*)&client_addr, &addr_len);
        setNonBlocking(clientSocket);
        if (clientSocket != -1) {
          FD_SET(clientSocket, &readFds);
          _clientSocks.push_back(clientSocket);
        }
      }
    }

    for (size_t i = 0; i < _clientSocks.size(); ++i) {
      int clientSocket = _clientSocks[i];

      if (FD_ISSET(clientSocket, &readFds)) {
        readFD(clientSocket);
        FD_CLR(clientSocket, &readFds);
        FD_SET(clientSocket, &writeFds);
      }
      if (FD_ISSET(clientSocket, &writeFds)) {
        writeFD(clientSocket, i);
      }
    }
  }
}

void WebServer::readFD(int fd) {
  int bytesReceived;
  unsigned char buffer[1024];
  std::vector<unsigned char> requestString;
  while (1) {
    bytesReceived = read(fd, buffer, sizeof(buffer));
    if (bytesReceived < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
        break;
      else {
        perror("read");
        break;
      }
    } else if (bytesReceived == 0) {
      break;
    }
    requestString.insert(requestString.end(), buffer, buffer + bytesReceived);
    memset(buffer, 0, sizeof(buffer));
  }
}

void WebServer::writeFD(int fd, int i) {
  std::string responseStr = "200 OK VLWS FLWS";
  send(fd, responseStr.c_str(), responseStr.length(), 0);
  FD_CLR(fd, &_writeFds);
  close(fd);
  _clientSocks.erase(_clientSocks.begin() + i);
}

void WebServer::setNonBlocking(int sock) {
  int opts;

  opts = fcntl(sock, F_GETFL);
  if (opts < 0) {
    perror("fcntl(F_GETFL)");
    exit(EXIT_FAILURE);
  }
  opts = (opts | O_NONBLOCK);
  if (fcntl(sock, F_SETFL, opts) < 0) {
    perror("fcntl(F_SETFL)");
    exit(EXIT_FAILURE);
  }
  return;
}