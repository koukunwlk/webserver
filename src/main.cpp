#include "Server/Server.hpp"
#include "webserver.hpp"

bool serverIsRunning = true;

int main(int argc, char** argv) {
  signal(SIGINT, SIG_IGN);
  if (argc < 2){
    std::cout << "Usage:\t./webserver <config_file.conf>\n";
    return EXIT_FAILURE;
  }
  std::cout << "Starting Webserver\n";

  std::string content;
  std::ifstream configFile(argv[1]);
  Parser parser;
  while (std::getline(configFile, content, '\n')) {
    if (content.size() == 0 || content[0] == '#') continue;
    parser.parseLine(content);
  }
  configFile.close();

  parser.populateServerConfigs();
  std::vector<ServerConfig> configs = parser.getServerConfigs();
  Server server = Server(configs);

  while (serverIsRunning) {
  }

}
