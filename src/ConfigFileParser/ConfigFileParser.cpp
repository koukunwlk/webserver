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
  } else {
    istringstream iss(line);
    vector<string> tokens;
    for (string s; iss >> s;) {
      tokens.push_back(s);
    }
    block->addProperty(tokens[0],
                       vector<string>(tokens.begin() + 1, tokens.end()));
  }
}

void Block::addChildBlock(Block block) { this->_childBlocks.push_back(block); }

void Block::addProperty(string key, vector<string> value) {
  Property property;
  property.first = key;
  property.second = value;

  this->_properties.push_back(property);
}

std::vector<Block> Block::getChildBlocks() { return this->_childBlocks; }

std::vector<Block> Parser::getBlocks() { return this->_blocks; }

string Block::getName() { return this->_name; }

Property Block::getNextProperty() {
  Property property = this->_properties.front();
  this->_properties.erase(this->_properties.begin());
  return property;
}