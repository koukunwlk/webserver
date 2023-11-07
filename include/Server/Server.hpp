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

extern bool serverIsRunning;



typedef struct ThreadArgs {
  std::vector<int> _fds;
  ServerConfig _srvConfig;
  sigset_t sigSet;
} threadArgs;

class Server {
 public:
  // Constructors
  Server();
  Server(std::vector<ServerConfig> config);
  // Destructor
  ~Server();

  // Methods
  void startRoutine(std::vector<ServerConfig> config);
  static void *thread(void *args);
  static void *signalThread(void *args);
  void    signalHandler();
  static int makeAFileDescriptorNonBlocking(int fd);
  static int makeAPortOnFileDescriptorReusable(int fd);


 private:
  std::vector<pthread_t> _threads;
  std::vector<int> _fds;
  ThreadArgs _threadArgs;
  sigset_t set;
};

#endif
