#include "webserver.hpp"
#include "Server/Server.hpp"

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  Server server = Server();

  if (server.setupServer() == EXIT_FAILURE)
    return 1;

  return 1;
}
