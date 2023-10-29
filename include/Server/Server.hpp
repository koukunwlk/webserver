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
#define MAX_HEADER 8192
#define PORT 5000

typedef struct ThreadArgs {
  int _epollFd;
  std::string _port;
  struct epoll_event *_epEvent;

} ThreadArgs;

class Server {
 public:
  // Constructors
  Server();
  // Destructor
  ~Server();

  // Methods
  int setupServer(int nServers);
  void fillThreadArgsStruct();
  void fillAddrStruct();
  int createEpollInstance();
  int createThreadPool(int serverQuantity);
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
