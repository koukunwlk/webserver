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
  std::vector<int> fds;
  ServerConfig currentServer;
  sigset_t sigSet;
  struct epoll_event *epEvent;
} ThreadArgs;

class Server {
 public:
  // Constructors
  Server();
  Server(std::vector<ServerConfig> config);
  // Destructor
  ~Server();

  // Methods
  int createThreadPool(std::vector<ServerConfig> config);
  static void *thread(void *args);
  static void *thread1(void *args);
  static int addListenFdToEpoll(int fd, int epollFd,
                                struct epoll_event *epEvent);
  static int makeAFileDescriptorNonBlocking(int fd);
  static int makeAPortOnFileDescriptorReusable(int fd);
  static int putFdToListen(struct sockaddr_in listenAddress);
  void signalHandler();
  static void *signalThread(void *args);

 private:
  ThreadArgs _tArgs;
  sigset_t set;
};

#endif
