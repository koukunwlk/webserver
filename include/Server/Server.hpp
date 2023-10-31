#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "webserver.hpp"

#define MAX_EVENTS 10

typedef struct ThreadArgs {
  int _epollFd;
  struct epoll_event *_epEvent;
  ServerConfig currentServer;
} ThreadArgs;

class Server {
 public:
  // Constructors
  Server();
  Server(std::vector<ServerConfig> config);
  // Destructor
  ~Server();

  // Methods
  void fillThreadArgsStruct();
  void fillAddrStruct();
  int createEpollInstance();
  int createThreadPool(std::vector<ServerConfig> config);
  static void *thread(void *args);
  static void *thread1(void *args);
  static int addListenFdToEpoll(int fd, int epollFd,
                                struct epoll_event *epEvent);
  void closeServer();
  static int makeAFileDescriptorNonBlocking(int fd);
  static int makeAPortOnFileDescriptorReusable(int fd);
  static int putFdToListen(struct sockaddr_in listenAddress);

 private:
  std::vector<pthread_t> _threads;
  ThreadArgs _tArgs;
  int _epollFd;
  struct epoll_event _epEvent;
};

#endif
