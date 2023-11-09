#include "WebServer/WebServer.hpp"

WebServer::WebServer() {
  FD_ZERO(&this->_readFds);
  FD_ZERO(&this->_writeFds);
}

WebServer::~WebServer() {}

void WebServer::raise() {
  // setup
  int optval = 1;

  int socketFd = socket(AF_INET, SOCK_STREAM, 0);

  _sockaddr.sin_family = AF_INET;
  _sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  _sockaddr.sin_port = htons(8000);

  setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  if (bind(socketFd, (const struct sockaddr*)&_sockaddr, sizeof(_sockaddr)))
    perror("bind");
  std::cout << "Listening on port: " << ntohs(_sockaddr.sin_port) << std::endl;

  if (listen(socketFd, 10)) perror("listen");

  if (fcntl(socketFd, F_SETFL, O_NONBLOCK)) perror("fcntl");

  FD_SET(socketFd, &_readFds);
  _listenSockets.push_back(socketFd);

  // run
  struct timeval timer;

  while (1) {
    timer.tv_sec = 1;
    timer.tv_usec = 0;
    fd_set readFds = _readFds;
    fd_set writeFds = _writeFds;

    if (select(FD_SETSIZE, &readFds, &writeFds, NULL, &timer)) perror("select");

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
        if (clientSocket != -1) {
          FD_SET(clientSocket, &readFds);
          _clientSocks.push_back(clientSocket);
        }
      }
    }

    for (size_t i = 0; i < _clientSocks.size(); ++i) {
      std::cout << "i: " << i << std::endl;
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
  unsigned char buffer[2];
  std::vector<unsigned char> requestString;
  while (1) {
    bytesReceived = recv(fd, buffer, sizeof(buffer), 0);
    if (bytesReceived < 0) {
      perror("recv");
    } else if (bytesReceived == 0) {
      std::cout << "asdasd\n";
      break;
    }
    requestString.insert(requestString.end(), buffer, buffer + bytesReceived);
    memset(buffer, 0, sizeof(buffer));
  }
  std::cout << "Request: " << std::endl;
  std::cout << requestString.data() << std::endl;
}

void WebServer::writeFD(int fd, int i) {
  std::string responseStr = "200 OK VLWS FLWS";
  send(fd, responseStr.c_str(), responseStr.length(), 0);
  close(fd);
  _clientSocks.erase(_clientSocks.begin() + i);
}