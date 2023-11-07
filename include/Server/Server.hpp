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

extern bool serverIsRunning;

typedef struct ThreadArgs {
  std::vector<pthread_t> threads;
  std::vector<int> fds;
  int _epollFd;
  struct epoll_event *_epEvent;
  ServerConfig currentServer;
  sigset_t sigSet;
  Request *request;
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
  void signalHandler();
  static void *signalThread(void *args);


 private:
  std::vector<pthread_t> _threads;
  std::vector<int> _fds;
  ThreadArgs _tArgs;
  int _epollFd;
  struct epoll_event _epEvent;
  sigset_t set;
};

#endif
