#include "Server/Server.hpp"

// Constructors
Server::Server() {}

Server::Server(std::vector<ServerConfig> config) {
  signalHandler();
  startRoutine(config);
}

// Destructor
Server::~Server() {}

// Methods
void Server::startRoutine(std::vector<ServerConfig> config) {
  pthread_t threadArray[config.size()];
  for (size_t i = 0; i < config.size(); i++) {
    this->_threadArgs._srvConfig = config[i];
    pthread_create(&threadArray[i], NULL, thread, (void *)&_threadArgs);
  }
}
void *Server::thread(void *args) {
  ThreadArgs *_tArgs = (ThreadArgs *)malloc(sizeof((ThreadArgs *)args));
  _tArgs = (ThreadArgs *)args;

  std::cout << "#########################################" << std::endl;
  std::cout << "#### Webserv is running on port " << _tArgs->_srvConfig.port
            << " ####" << std::endl;
  std::cout << "#########################################" << std::endl;

  int listenSocket;
  struct sockaddr_in listenSocketServerAddress;
  listenSocketServerAddress.sin_family = AF_INET;
  listenSocketServerAddress.sin_port = htons(_tArgs->_srvConfig.port);
  listenSocketServerAddress.sin_addr.s_addr = inet_addr("0.0.0.0");

  if ((listenSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cerr << "Error creating socket" << std::endl;
    return NULL;
  }
  makeAPortOnFileDescriptorReusable(listenSocket);
  makeAFileDescriptorNonBlocking(listenSocket);
  if (bind(listenSocket, (struct sockaddr *)&(listenSocketServerAddress),
           sizeof(listenSocketServerAddress)) < 0) {
    std::cerr << "Error binding socket" << std::endl;
    return NULL;
  }
  if (listen(listenSocket, 64) < 0) {
    std::cerr << "Error listening on socket" << std::endl;
    return NULL;
  }
  
  _tArgs->_fds.push_back(listenSocket);

  int clientFd;
  struct sockaddr_in clientAddress;
  socklen_t clientAddressLength = sizeof(clientAddress);
  while (serverIsRunning) {
    std::cout << "Socket [" << listenSocket << "] Waiting for connections..."
              << std::endl;

    clientFd = accept(listenSocket, (struct sockaddr *)&clientAddress,
                      &clientAddressLength);
    if (clientFd < 0) {
        std::cerr << "Error accepting connection" << std::endl;
      break ;
    }

    int bytesReceived;
    unsigned char buffer[1024];
    std::vector<unsigned char> requestString;
    while (1) {
      bytesReceived = read(clientFd, buffer, sizeof(buffer));
      if (bytesReceived < 0) {
        if (bytesReceived == -1 &&
            fcntl(clientFd, F_GETFL, O_NONBLOCK, FD_CLOEXEC))
          break;
        else
          perror("read");
      } else if (bytesReceived == 0)
        break;
      requestString.insert(requestString.end(), buffer, buffer + bytesReceived);
      memset(buffer, 0, sizeof(buffer));
    }
    Request request(requestString, _tArgs->_srvConfig);
    Handler handler(request);

    Response response = handler.getResponse();
    std::stringstream responseString;
    responseString << response;
    write(clientFd, responseString.str().c_str(),
          responseString.str().length());
    close(clientFd);
  }

  return NULL;
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

  if (err) {
    perror("Error waiting for signal");
  } else {
    std::cout << "\nSignal received, closing the server. " << sig << std::endl;
    for (size_t i = 0; i < tArgs->_fds.size(); i++) {
      close(tArgs->_fds[i]);
    }
    serverIsRunning = false;
  }
  return NULL;
}

void Server::signalHandler() {
  pthread_t thread;
  sigemptyset(&this->set);
  sigaddset(&this->set, SIGINT);
  pthread_sigmask(SIG_BLOCK, &set, 0);

  this->_threadArgs.sigSet = set;
  pthread_create(&thread, NULL, signalThread,
                 static_cast<void *>(&this->_threadArgs));
  this->_threads.push_back(thread);
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
