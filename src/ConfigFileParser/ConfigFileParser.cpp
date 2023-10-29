#include "ConfigFileParser/ConfigFileParser.hpp"
using namespace std;

Block::Block(string name) { _name = name; }

Block::Block() {}

Block::~Block() {}

Parser::Parser() {}

Parser::~Parser() {}

Block* Parser::createBlock(string line) {
  istringstream iss(line);
  vector<string> tokens;
  for (string s; iss >> s;) {
    tokens.push_back(s);
  }

  if (tokens[0].compare("location") != 0 && tokens.size() != 2) {
    throw ParsingException("Invalid block definition '" + line + "'");
  }

  Block::isValidBlockDefinition(tokens[0]);

  Block* block = new Block(tokens[0]);
  if (tokens[0].compare("location") == 0) {
    block->setProperty(Property("url", vector<string>(1, tokens[1])));
    if (tokens.size() == 4)
      block->setProperty(Property("extension", vector<string>(1, tokens[2])));
  }

  return block;
}

void Block::isValidBlockDefinition(string name) {
  if (name.compare("server") != 0 && name.compare("location") != 0) {
    throw ParsingException("Invalid block name " + name);
  }
}

void Parser::parseLine(string line) {
  static Block* block;
  static Block* childBlock;
  line = str_trim(line);
  line = removeComments(line);
  if (line.find("{") != string::npos) {
    if (static_cast<void*>(block) != NULL) {
      childBlock = Parser::createBlock(line);
    } else {
      block = Parser::createBlock(line);
    }
  } else if (line.compare("}") == 0) {
    if (static_cast<void*>(childBlock) != NULL) {
      block->addChildBlock(*childBlock);
      childBlock = NULL;
    } else {
      this->_blocks.push_back(*block);
      block = NULL;
    }
  } else if (line.size() != 0) {
    istringstream iss(line);
    vector<string> tokens;
    for (string s; iss >> s;) {
      tokens.push_back(s);
    }
    if (static_cast<void*>(childBlock) != NULL)
      Parser::addProperty(tokens[0],
                          vector<string>(tokens.begin() + 1, tokens.end()),
                          childBlock);
    else if (static_cast<void*>(block) != NULL)
      Parser::addProperty(
          tokens[0], vector<string>(tokens.begin() + 1, tokens.end()), block);
  }
}

void Block::addChildBlock(Block block) { this->_childBlocks.push_back(block); }

void Parser::addProperty(string key, vector<string> value, Block* block) {
  Property property;
  property.first = key;
  property.second = value;
  block->setProperty(property);
}

std::vector<Block> Block::getChildBlocks() { return this->_childBlocks; }

std::vector<Block> Parser::getBlocks() { return this->_blocks; }

string Block::getName() { return this->_name; }

Property Block::getNextProperty() {
  Property property = this->_properties.front();
  this->_properties.erase(this->_properties.begin());
  return property;
}

void Block::setProperty(Property property) {
  this->_properties.push_back(property);
}

void Parser::populateServerConfigs() {
  std::vector<Block> blocks = this->getBlocks();

  for (size_t i = 0; i < blocks.size(); i++) {
    createServer(blocks[i]);
  }
}

std::vector<Property> Block::getProperties() { return this->_properties; }

void Parser::createServer(Block server) {
  struct ServerConfig serverConfig;
  std::vector<Property> properties = server.getProperties();
  vector<Block> childBlocks = server.getChildBlocks();

  for (size_t i = 0; i < properties.size(); i++) {
    Property currentProperty = properties[i];
    if (currentProperty.first.compare("listen") == 0) {
      std::istringstream(currentProperty.second[0]) >> serverConfig.port;
    }
    if (currentProperty.first.compare("server_name") == 0) {
      serverConfig.server_names = currentProperty.second;
    }

    if (currentProperty.first.compare("client_max_body_size") == 0) {
      serverConfig.client_max_body_size = currentProperty.second[0];
    }

    if (currentProperty.first.compare("location") == 0) {
      createLocation(server);
    }
  }
  if (childBlocks.size() != 0) {
    for (size_t i = 0; i < childBlocks.size(); i++) {
      serverConfig.locations.push_back(createLocation(childBlocks[i]));
    }
  }
  this->setServerConfig(serverConfig);
}

Location Parser::createLocation(Block location) {
  Location locationConfig;
  std::vector<Property> properties = location.getProperties();

  for (size_t i = 0; i < properties.size(); i++) {
    Property currentProperty = properties[i];
    if (currentProperty.first.compare("url") == 0) {
      locationConfig.url = currentProperty.second[0];
    }

    if (currentProperty.first.compare("extension") == 0) {
      locationConfig.extension = currentProperty.second[0];
    }

    if (currentProperty.first.compare("root") == 0) {
      locationConfig.root = currentProperty.second[0];
    }

    if (currentProperty.first.compare("index") == 0) {
      locationConfig.index = currentProperty.second[0];
    }

    if (currentProperty.first.compare("error_page") == 0) {
      ErrorPage errorPage;
      std::istringstream(currentProperty.second[0]) >> errorPage.code;
      errorPage.path = currentProperty.second[1];
      locationConfig.error_page.push_back(errorPage);
    }

    if (currentProperty.first.compare("allow") == 0) {
      locationConfig.methods = currentProperty.second;
    }
  }
  return locationConfig;
}

void Parser::setServerConfig(ServerConfig serverConfig) {
  this->_serverConfigs.push_back(serverConfig);
}

vector<ServerConfig> Parser::getServerConfigs() { return this->_serverConfigs; }

std::string str_trim(const std::string& str) {
  std::string result = str;
  size_t start = result.find_first_not_of(" \t\n\r");
  if (start != std::string::npos) {
    result = result.substr(start);
  }

  size_t end = result.find_last_not_of(" \t\n\r");
  if (end != std::string::npos) {
    result = result.substr(0, end + 1);
  }

  return result;
}

std::string removeComments(std::string line) {
  size_t comment = line.find("#");
  if (comment != std::string::npos) {
    line = line.substr(0, comment);
  }
  return line;
}