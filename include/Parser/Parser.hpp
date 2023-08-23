#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include "WebServerException/WebServerException.hpp"

typedef std::vector<std::string> Tokens;
typedef std::map<std::string, Tokens> Block;

class Parser {
 public:
  Parser();
  ~Parser();
  void parseLine(std::string line);
  Block::iterator getNextServerBlock();

 private:
  Block::iterator currentBlock;
  std::vector<std::string> _symbols;
  Block _blocks;

  bool isBlock(std::string line);
  bool validateBlockDefinition(std::string line) throw(ParsingException);
};