#include "Server/Server.hpp"

#include "Request/Request.hpp"
#include "gtest/gtest.h"

TEST(ServerClass, setupServerTest) {
  Server server;

  EXPECT_EQ(server.setupServer(), EXIT_SUCCESS);
}

TEST(ServerClass, createEpollInstance) {
  Server server;

  EXPECT_EQ(server.createEpollInstance(), EXIT_SUCCESS);
}

TEST(ServerClass, addListenFdToEpollBeforeEpollInstance) {
  Server server;

  EXPECT_EQ(server.addListenFdToEpoll(), EXIT_FAILURE);
}

TEST(ServerClass, addListenFdToEpollAfterEpollInstance) {
  Server server;

  server.setupServer();
  server.createEpollInstance();

  EXPECT_EQ(server.addListenFdToEpoll(), EXIT_SUCCESS);
}

TEST(ServerClass, AcceptConnectionFromClientAndWritingOnStdOut) {
  Server server;

  server.setupServer();
  EXPECT_EQ(server.addListenFdToEpoll(), EXIT_FAILURE);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
