#pragma once
#include <algorithm>
#include <iostream>
#include <sys/stat.h>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "WebServerException/WebServerException.hpp"

class Block;

typedef std::pair<std::string, std::vector<std::string> > Property;

struct ErrorPage {
  int code;
  std::string path;
};

struct Location {
  std::string url;
  std::string extension;

  std::string root;
  std::vector<std::string> index;
  bool autoindex;
  std::string redirect;
  std::vector<ErrorPage> error_page;
  std::vector<std::string> methods;
  std::string upload_store;
};

struct ServerConfig {
  int port;
  std::vector<std::string> server_names;
  int client_max_body_size;
  std::vector<Location> locations;
};

class Block {
 public:
  Block();
  Block(std::string name);
  ~Block();
  std::string getName();
  std::vector<Block> getChildBlocks();
  Property getNextProperty();
  std::vector<Property> getProperties();

  void addChildBlock(Block block);
  static void isValidBlockDefinition(std::string line);
  void setProperty(Property property);

 private:
  std::string _name;
  std::vector<Property> _properties;
  std::vector<Block> _childBlocks;
};

class Parser {
 public:
  Parser();
  ~Parser();
  void parseLine(std::string line);
  static Block* createBlock(std::string line);
  void addProperty(std::string key, std::vector<std::string> value,
                   Block* block);
  std::vector<Block> getBlocks();
  std::vector<ServerConfig> getServerConfigs();
  void populateServerConfigs();
  void setServerConfig(ServerConfig serverConfigs);

 private:
  std::vector<Block> _blocks;
  std::vector<ServerConfig> _serverConfigs;

  Location createLocation(Block location);
  void createServer(Block server);
};

std::string str_trim(const std::string& str);
std::string removeComments(std::string line);
int convertSize(std::string size);

/*
  * 1. string no formato      "GET /teste.php HTTP/1.1\r\n"
                              "Host: localhost:3000\n"
                              "Content-Type text/plain\n"
                              "Content-Length: 16\r\n\r\n"
                              "body muito doido"
  * 2. as informações utilizadas para a validação da location sera, o metodo, o
  endpoint e a extensão do arquivo

  * 3. Depois de validar preencher as informações em uma classe Request
  * 4. Caso a url não bata com nenhuma location, popular a classe Request com a

 */