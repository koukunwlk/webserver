#include "Server/Server.hpp"

// Constructors
Server::Server() {
  fillAddrStruct(); }

// Destructor
Server::~Server() { closeServer(); }

// Methods
int Server::setupServer() {
  if (createSocketConnection() == -1) return EXIT_FAILURE;
  if (makeAFileDescriptorNonBlocking()) return EXIT_FAILURE;
  if (makeAPortOnFileDescriptorReusable()) return EXIT_FAILURE;
  if (bindSocket()) return EXIT_FAILURE;
  if (putSocketToListen()) return EXIT_FAILURE;
  if (createEpollInstance()) return EXIT_FAILURE;
  if (addListenFdToEpoll()) return EXIT_FAILURE;
  fillThreadArgsStruct();
  if (createThreadPool()) return EXIT_FAILURE;
  // std::cout << "Server started" << std::endl;
  return EXIT_SUCCESS;
}

void Server::closeServer() { close(this->_listenFd);
  close(this->_epollFd);}

int Server::createSocketConnection() {
  if ((this->_listenFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cerr << "Error creating socket" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int Server::makeAPortOnFileDescriptorReusable() {
  int on = 1;
  if(setsockopt(this->_listenFd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
    std::cerr << "Error setting socket options" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int Server::createThreadPool() {
  for (int i = 0; i < 2; i++) {
    if (pthread_create(&this->threads[i], NULL, thread, &this->_tArgs) == -1) {
      std::cerr << "Error creating thread" << std::endl;
      return EXIT_FAILURE;
    }
  }
  thread(&this->_tArgs);
  return EXIT_SUCCESS;
}

int Server::makeAFileDescriptorNonBlocking() {
  if (fcntl(this->_listenFd, F_SETFL, O_NONBLOCK) == -1) {
    std::cerr << "Error making file descriptor non-blocking" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void *Server::thread(void *args) {
  int epollFd = ((ThreadArgs *)args)->epollFd;
  long listenFd = ((ThreadArgs *)args)->listenFd;

  struct epoll_event ev;
  struct epoll_event *events =
      (struct epoll_event *)malloc(sizeof(struct epoll_event) * MAX_EVENTS);
  if (events == NULL) {
    std::cerr << "Error allocating events" << std::endl;
  }

  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);

  int numberToFds;
  while (1) {
    numberToFds = epoll_wait(epollFd, events, MAX_EVENTS, -1);
    if (numberToFds <= 0) {
      std::cerr << "Error on epoll_wait" << std::endl;
    }
    for (int i = 0; i < numberToFds; i++) {
      if (events[i].data.fd == listenFd) {
        while (1) {
          int clientFd =
              accept(listenFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
          if (clientFd == -1) {
            std::cerr << "Error on accept" << std::endl;
          }
          fcntl(clientFd, F_SETFL, O_NONBLOCK);
          ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;

          // CREATE REQUEST
          char buffer[1024];
          memset(buffer, 0, sizeof(buffer));
          int bytesReceived;
          std::string s;
          while ((bytesReceived = read(clientFd, buffer, sizeof(buffer)))) {
            s += buffer;
          }
          Request request = Request(s.c_str());
          std::cout << request.getRawData() << std::endl;

          if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &ev) == -1) {
            std::cerr << "Error on epoll_ctl" << std::endl;
            continue;
          }
        }
      }
    }
    // free(events);
  }
}

void Server::fillThreadArgsStruct() {
  this->_tArgs.epollFd = this->_epollFd;
  this->_tArgs.listenFd = this->_listenFd;
}

void Server::fillAddrStruct() {
  this->_listenAddress.sin_family = AF_INET;
  this->_listenAddress.sin_port = htons(PORT);
  this->_listenAddress.sin_addr.s_addr = inet_addr("0.0.0.0");
}

int Server::bindSocket() {
  if (bind(this->_listenFd, (struct sockaddr *)&_listenAddress,
           sizeof(_listenAddress)) == -1) {
    std::cerr << "Error binding socket" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int Server::putSocketToListen() {
  if (listen(this->_listenFd, 64) == -1) {
    std::cerr << "Error putting socket to listen" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int Server::createEpollInstance() {
  if ((this->_epollFd = epoll_create1(0)) == -1) {
    std::cerr << "Error creating epoll instance" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int Server::addListenFdToEpoll() {
  this->_epEvent.events = EPOLLIN | EPOLLOUT | EPOLLET;
  this->_epEvent.data.fd = this->_listenFd;
  if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, this->_listenFd,
                &this->_epEvent) == -1) {
    std::cerr << "Error adding socket to epoll" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
