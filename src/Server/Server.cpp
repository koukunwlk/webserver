#include "Server/Server.hpp"

// Constructors
Server::Server() {}

pthread_t *Server::_threadEpoll = NULL;
int Server::serverQuantity = 0;
pthread_t *Server::_signalThread = NULL;

Server::Server(std::vector<ServerConfig> config) {
  serverIsRunning = true;
  signalHandler();
  createThreadPool(config);
}

// Destructor
Server::~Server() {
  for (int i = 0; i < Server::serverQuantity; i++) {
    pthread_join(Server::_threadEpoll[i], NULL);
  }
  pthread_join(*Server::_signalThread, NULL);
  free(Server::_threadEpoll);
  free(Server::_signalThread);
}

// Methods
int Server::createThreadPool(std::vector<ServerConfig> config) {
  int serverQuantity = config.size();
  pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * serverQuantity);
  Server::serverQuantity = serverQuantity;

  for (int i = 0; i < serverQuantity; i++) {
    ServerConfig currentServer = config[i];
    this->_tArgs.currentServer = currentServer;
    if (pthread_create(&threads[i], NULL, thread, &this->_tArgs) == -1) {
      perror("PTHREAD_CREATE: ");
      return EXIT_FAILURE;
    }
    sleep(1);
  }
  Server::_threadEpoll = threads;
  return EXIT_SUCCESS;
}

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

  if (listen(fd, 500) == -1) {
    perror("Error putting socket to listen");
    return -1;
  }

  std::cout << "Listening on port: " << ntohs(listenAddress.sin_port)
            << std::endl;

  return fd;
}

void Server::signalHandler() {
  sigset_t sigs;
  sigemptyset(&sigs);
  sigaddset(&sigs, SIGINT);
  pthread_sigmask(SIG_BLOCK, &sigs, NULL);
  pthread_t *thread = (pthread_t *)malloc(sizeof(pthread_t));
  if (pthread_create(thread, NULL, signalThread, &this->_tArgs) == -1) {
    perror("PTHREAD_CREATE: ");
  }
  Server::_signalThread = thread;
}

void *Server::signalThread(void *args) {
  ThreadArgs *tArgs = (ThreadArgs *)args;
  int sig;
  int err;
  sigset_t sigs;
  sigset_t oldSigSet;

  sigemptyset(&sigs);
  sigaddset(&sigs, SIGINT);

  pthread_sigmask(SIG_BLOCK, &sigs, &oldSigSet);
  err = sigwait(&sigs, &sig);
  std::cout << "Signal received: " << sig << std::endl;
  if (err) {
    perror("SIGWAIT: ");
  } else {
    if (sig == SIGINT) {
      for (size_t i = 0; i < tArgs->fds.size(); i++) {
        int value = close(tArgs->fds[i]);
        if (value < 0) {
          if (value == -1 && fcntl(tArgs->fds[i], F_GETFL, O_NONBLOCK)) {
            i++;
            continue;
          } else
            perror("CLOSE FD ON SIGNAL: ");
        }
      }
      serverIsRunning = false;
    }
  }
  return NULL;
}

void *Server::thread(void *args) {
  ThreadArgs *tArgs = (ThreadArgs *)args;
  int epollFd;
  struct epoll_event *epEvent =
      (struct epoll_event *)malloc(sizeof(struct epoll_event) * MAX_EVENTS);
  tArgs->epEvent = epEvent;
  if ((epollFd = epoll_create1(0)) == -1) {
    perror("EPOLL_CREATE: ");
  }

  ServerConfig currentServer = ((ThreadArgs *)args)->currentServer;

  int port = currentServer.port;
  tArgs->fds.push_back(epollFd);

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
    readyFds = epoll_wait(epollFd, epEvent, MAX_EVENTS, 1000);
    for (int i = 0; i < readyFds; i++) {
      if (epEvent[i].data.fd == listenFd) {
        clientFd =
            accept(listenFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientFd < 0) {
          if (clientFd == -1 &&
              fcntl(epEvent[i].data.fd, F_GETFL, O_NONBLOCK, FD_CLOEXEC))
            break;
          else {
            perror("ACCEPT: ");
            break;
          }
        }
        tArgs->fds.push_back(epollFd);
        makeAFileDescriptorNonBlocking(clientFd);
        ev.events = EPOLLIN;
        ev.data.fd = clientFd;
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &ev) < 0) {
          perror("EPOL_CTL_ADD: ");
        }
      } else {
        int bytesReceived;
        unsigned char buffer[1024];
        std::vector<unsigned char> requestString;
        while (1) {
          bytesReceived = read(epEvent[i].data.fd, buffer, sizeof(buffer));
          if (bytesReceived < 0) {
            if (bytesReceived == -1 &&
                fcntl(epEvent[i].data.fd, F_GETFL, O_NONBLOCK, FD_CLOEXEC))
              break;
            else
              perror("READ: ");
          } else if (bytesReceived == 0)
            break;
          requestString.insert(requestString.end(), buffer,
                               buffer + bytesReceived);
          memset(buffer, 0, sizeof(buffer));
        }

        Request request(requestString, currentServer);
        Handler handle(request);
        Response response = handle.getResponse();
        std::stringstream responseStr;
        responseStr << response;

        write(epEvent[i].data.fd, responseStr.str().c_str(),
              responseStr.str().length());
        close(epEvent[i].data.fd);
      }
    }
  }
  free(epEvent);
  return NULL;
}

int Server::addListenFdToEpoll(int fd, int epollFd,
                               struct epoll_event *epEvent) {
  epEvent->events = EPOLLIN;
  epEvent->data.fd = fd;

  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, epEvent) == -1) {
    perror("EPOLL_CTL: ");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int Server::makeAPortOnFileDescriptorReusable(int fd) {
  int on = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
    perror("SETSOCKOPT SO_REUSEADDR: ");
    return EXIT_FAILURE;
  }
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, (char *)&on, sizeof(on)) < 0) {
    perror("SETSOCKOPT SO_REUSEADDR: ");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int Server::makeAFileDescriptorNonBlocking(int fd) {
  if (fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
    perror("FCNTL NONBLOCK: ");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
