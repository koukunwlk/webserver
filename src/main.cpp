#include "Server/Server.hpp"
#include "webserver.hpp"

int main(int argc, char** argv) {
  if (argc < 2) return EXIT_FAILURE;

  std::string content;
  std::ifstream configFile(argv[1]);
  Parser* parser = new Parser;
  while (std::getline(configFile, content, '\n')) {
    if (content.size() == 0 || content[0] == '#') continue;
    parser->parseLine(content);
  }
  configFile.close();

  parser->populateServerConfigs();
  std::vector<ServerConfig> configs = parser->getServerConfigs();
  Server server = Server(configs);

  while (1) {
  }
  
}
