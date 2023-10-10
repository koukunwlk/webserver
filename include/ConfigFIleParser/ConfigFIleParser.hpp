#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include "WebServerException/WebServerException.hpp"

class Block;

typedef std::map<std::string, std::vector<std::string> > Property;

class Block {
 public:
  Block();
  Block(std::string name);
  ~Block();
  std::string getName();
  std::vector<Block> getChildBlocks();
  Property getNextProperty();
  void addProperty(std::string key, std::vector<std::string>  value);
  void addChildBlock(Block block);
  static void isValidBlockDefinition(std::string line) throw(ParsingException);

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
  Block* createBlock(std::string line);

 private:
  std::vector<Block> _blocks;
};