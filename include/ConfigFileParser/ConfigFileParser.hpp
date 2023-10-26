#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "WebServerException/WebServerException.hpp"

class Block;

typedef std::pair<std::string, std::vector<std::string> > Property;

struct error_page {
  int code;
  std::string path;
};

struct Location {
  std::string url;
  std::string extension;

  std::string root;
  std::string index;
  bool autoindex;
  std::string redirect;
  std::vector<error_page> error_page;
  std::vector<std::string> methods;
};

struct ServerConfig {
  int port;
  std::vector<std::string> server_names;
  std::string root;
  std::vector<std::string> index;
  std::vector<error_page> error_page;
  std::string client_max_body_size;
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

 private:
  std::vector<Block> _blocks;
  std::vector<ServerConfig> _serverConfigs;
};
