#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>

#include <iostream>
#include <string>

#include "ConfigFileParser/ConfigFileParser.hpp"
#include "Handler/Handler.hpp"
#include "Request/Request.hpp"
#include "Response/Response.hpp"
#include "Server/Server.hpp"
#include "WebServerException/WebServerException.hpp"

class WebServer {
 public:
  WebServer();
  ~WebServer();

  int cookSocket();
  void raise();
  void readFD(int fd);
  void writeFD(int fd, int epollMonitor);
  void setNonBlocking(int sock);
  void acceptConnection(int fd, int listenFd);

 private:
  sockaddr_in _sockaddr;
  std::vector<int> _listenSockets;
  std::vector<int> _clientSocks;
  fd_set _readFds;
  fd_set _writeFds;
};

void addFdToEpoll(int fd, int epollFdm);

#endif /* ***************************************************** WEBSERV_H */
