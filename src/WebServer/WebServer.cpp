#include "WebServer/WebServer.hpp"

WebServer::WebServer() {
  FD_ZERO(&this->_readFds);
  FD_ZERO(&this->_writeFds);
}

WebServer::~WebServer() {}

int WebServer::cookSocket() {
  _sockaddr.sin_family = AF_INET;
  _sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  _sockaddr.sin_port = htons(8000);

  int listenFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  if (bind(listenFd, (const struct sockaddr*)&_sockaddr, sizeof(_sockaddr)))
    perror("bind");
  std::cout << "Listening on port: " << ntohs(_sockaddr.sin_port) << std::endl;

  if (listen(listenFd, 64)) perror("listen");

  setNonBlocking(listenFd);
  _listenSockets.push_back(listenFd);

  return 0;
}

void WebServer::raise() {
  // setup
  cookSocket();
  int eventCount;
  struct epoll_event event, events[64];
  int epollMonitor = epoll_create1(0);

  event.data.fd = _listenSockets[0];
  event.events = EPOLLIN | EPOLLOUT | EPOLLET;

  epoll_ctl(epollMonitor, EPOLL_CTL_ADD, _listenSockets[0], &event);

  while (1) {
    eventCount = epoll_wait(epollMonitor, events, 64, 1000);
    for (int i = 0; i < eventCount; ++i) {
      if (events[i].data.fd == _listenSockets[0]) {
        acceptConnection(epollMonitor, _listenSockets[0]);
      } else if (events[i].events & EPOLLIN) {
        readFD(events[i].data.fd);
        writeFD(events[i].data.fd, epollMonitor);
      } else {
        std::cout << "Unknown event" << std::endl;
      }
      if (events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
        printf("[+] connection closed\n");
        epoll_ctl(epollMonitor, EPOLL_CTL_DEL, events[i].data.fd, NULL);
        close(events[i].data.fd);
        continue;
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

void WebServer::writeFD(int fd, int epollMonitor) {
  std::string responseStr = "200 OK VLWS FLWS";
  write(fd, responseStr.c_str(), responseStr.length());
  epoll_event ev;
  ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP;
  ev.data.fd = fd;
  epoll_ctl(epollMonitor, EPOLL_CTL_DEL, fd, &ev);
  close(fd);
}

void WebServer::setNonBlocking(int sock) {
  int old_option = fcntl(sock, F_GETFL);
  int new_option = old_option | O_NONBLOCK;

  fcntl(sock, F_SETFL, new_option);
}

void WebServer::acceptConnection(int epollFd, int listenFd) {
  int clientFd;
  struct sockaddr_in clientAddr;
  socklen_t clientAddrSize = sizeof(clientAddr);
  clientFd = accept(listenFd, (struct sockaddr*)&clientAddr, &clientAddrSize);
  setNonBlocking(clientFd);
  addFdToEpoll(clientFd, epollFd);
}

void addFdToEpoll(int fd, int epollFdm) {
  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLET;
  ev.data.fd = fd;
  if (epoll_ctl(epollFdm, EPOLL_CTL_ADD, fd, &ev) < 0) {
    perror("epoll_ctl error ");
  }
}