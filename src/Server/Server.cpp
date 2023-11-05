#include "Server/Server.hpp"

// Constructors
Server::Server() {}

Server::Server(std::vector<ServerConfig> config) {
  signalHandler();
  createThreadPool(config);

 }

// Destructor
Server::~Server() { closeServer(); }

// Methods
int Server::createThreadPool(std::vector<ServerConfig> config) {
  int serverQuantity = config.size();
  pthread_t threads[serverQuantity];

  for (int i = 0; i < serverQuantity; ++i) {
    ServerConfig currentServer = config[i];
    this->_tArgs.currentServer = currentServer;
    if (pthread_create(&threads[i], NULL, thread, &this->_tArgs) == -1) {
      std::cerr << "Error creating thread" << std::endl;
      return EXIT_FAILURE;
    }
    this->_tArgs.threads.push_back(threads[i]);
    sleep(1);
  }

  return EXIT_SUCCESS;
}

void Server::closeServer() { }

int Server::putFdToListen(struct sockaddr_in listenAddress) {
  int fd;

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Error creating socket");
    return -1;
  }

  makeAFileDescriptorNonBlocking(fd);
  makeAPortOnFileDescriptorReusable(fd);

  if (bind(fd, (struct sockaddr *)&listenAddress, sizeof(listenAddress)) ==
      -1) {
    perror("Error binding socket");
    return -1;
  }

  if (listen(fd, 64) == -1) {
    perror("Error putting socket to listen");
    return -1;
  }

  std::cout << "Listening on port: " << ntohs(listenAddress.sin_port)
            << std::endl;

  return fd;
}


void Server::signalHandler() {
  pthread_t thread;
  sigemptyset(&this->set);
  sigaddset(&this->set, SIGINT);
  pthread_sigmask(SIG_BLOCK, &set, 0);

  this->_tArgs.sigSet = set;
  pthread_create(&thread, NULL, signalThread, static_cast<void *>(&
  this->_tArgs));
  this->_threads.push_back(thread);
}

void *Server::signalThread(void *args) {
  ThreadArgs *tArgs = (ThreadArgs *)args;
  int sig;
  int err;
  sigset_t sigs;
  sigset_t oldSigSet;
  sigfillset(&sigs);
  pthread_sigmask(SIG_BLOCK, &sigs, &oldSigSet);

  err = sigwait(&tArgs->sigSet, &sig);

  if(err) {
    std::cerr << "Error on sigwait" << std::endl;
  } else {
    std::cout << "Caught signal: " << sig << std::endl;
    for (size_t i = 0; i < tArgs->fds.size(); i++) {
      close(tArgs->fds[i]);
    }
    serverIsRunning = false;
  }
  return NULL;
}

void *Server::thread(void *args) {
  ThreadArgs *tArgs = (ThreadArgs *)args;
  int epollFd;
  struct epoll_event *epEvent =
      (struct epoll_event *)malloc(sizeof(struct epoll_event) * MAX_EVENTS);
  if ((epollFd = epoll_create1(0)) == -1) {
    perror("Error creating epoll instance");
  }
  tArgs->fds.push_back(epollFd);
  int port = ((ThreadArgs *)args)->currentServer.port;

  struct epoll_event ev;

  int listenFd;
  struct sockaddr_in listenAddr;
  listenAddr.sin_family = AF_INET;
  listenAddr.sin_port = htons(port);
  listenAddr.sin_addr.s_addr = inet_addr("0.0.0.0");

  listenFd = putFdToListen(listenAddr);
  addListenFdToEpoll(listenFd, epollFd, epEvent);
  tArgs->fds.push_back(listenFd);
  int clientFd;
  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);

  int readyFds;
  while (serverIsRunning) {
    readyFds = epoll_wait(epollFd, epEvent, MAX_EVENTS, -1);
    if (readyFds <= 0) {
      std::cerr << "Error on epoll_wait" << std::endl;
    }
    for (int i = 0; i < readyFds; i++) {
      if (epEvent[i].data.fd == listenFd) {
        clientFd =
            accept(listenFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientFd < 0) {
          if (errno == EAGAIN || errno == EWOULDBLOCK)
            break;
          else {
            perror("Accept error");
            break;
          }
        }
        tArgs->fds.push_back(epollFd);

        makeAFileDescriptorNonBlocking(clientFd);
        ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
        ev.data.fd = clientFd;

        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &ev) < 0) {
          perror("epoll_ctl error ");
        }
      } else {
        int bytesReceived;
        unsigned char buffer[1024];
        std::vector<unsigned char> requestString;
        while (1) {
          bytesReceived = read(clientFd, buffer, sizeof(buffer));
          if (bytesReceived < 0) {
            if (bytesReceived == -1 && fcntl(clientFd, F_GETFL, O_NONBLOCK, FD_CLOEXEC))
              break;
            else
              perror("read");
          } else if (bytesReceived == 0)
            break;
          requestString.insert(requestString.end(), buffer, buffer + bytesReceived);
          memset(buffer, 0, sizeof(buffer));
        }
          // std::cout << requestString.data() << std::endl;
          Request request(requestString);
          if (epoll_ctl(epollFd, EPOLL_CTL_MOD, clientFd, &ev) < 0) {
            perror("epoll_ctl error ");
          }
          request.setServerRoot("www");
          Handler handle(request);
          Response response = handle.getResponse();
          std::stringstream responseStr;
          responseStr << response;
          write(clientFd, responseStr.str().c_str(), responseStr.str().length());
          if (epoll_ctl(epollFd, EPOLL_CTL_DEL, clientFd, NULL) == -1)
            perror("Error while delete clientFd from Epol");
          close(clientFd);
      }
    }
  }
  free(epEvent);
  close(listenFd);
  return NULL;
}

int Server::createEpollInstance() {
  if ((this->_epollFd = epoll_create1(0)) == -1) {
    perror("Error creating epoll instance");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int Server::addListenFdToEpoll(int fd, int epollFd,
                               struct epoll_event *epEvent) {
  epEvent->events = EPOLLIN | EPOLLET;
  epEvent->data.fd = fd;

  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, epEvent) == -1) {
    std::cerr << "Error adding socket to epoll" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int Server::makeAPortOnFileDescriptorReusable(int fd) {
  int on = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
    std::cerr << "Error setting socket option SO_REUSEADDR" << std::endl;
    return EXIT_FAILURE;
  }
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, (char *)&on, sizeof(on)) < 0) {
    std::cerr << "Error setting socket options SO_REUSEPORT" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int Server::makeAFileDescriptorNonBlocking(int fd) {
  if (fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
    std::cerr << "Error making file descriptor non-blocking" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
