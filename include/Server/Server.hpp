#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>


#include "Request/Request.hpp"
#include "WebServerException/HostingException.hpp"

#define MAX_EVENTS 10
#define PORT 8080

typedef struct ThreadArgs {
  int epollFd;
  int listenFd;
} ThreadArgs;

class Server {
 public:
  // Constructors
  Server();
  // Destructor
  ~Server();

  // Methods
  int createSocketConnection();
  int makeAPortOnFileDescriptorReusable();
  int createThreadPool();
  void fillThreadArgsStruct();
  static void *thread(void *args);
  int makeAFileDescriptorNonBlocking();
  void closeServer();
  void fillAddrStruct();
  int setupServer();
  int bindSocket();
  int putSocketToListen();
  int createEpollInstance();
  int addListenFdToEpoll();

 private:
  pthread_t threads[2];
  ThreadArgs _tArgs;
  int _listenFd;
  int _epollFd;
  struct sockaddr_in _listenAddress;
  struct epoll_event _epEvent;
};

#endif
