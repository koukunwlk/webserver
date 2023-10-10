#include "ConfigFIleParser/ConfigFIleParser.hpp"
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

void Block::isValidBlockDefinition(string name) throw (ParsingException) {
  if (name.compare("server") != 0 && name.compare("location") != 0) {
    throw ParsingException("Invalid block name " + name);
  }
}

void Parser::parseLine(string line) {
  Block *block;
  Block *childBlock;
  if (line.find("{") != string::npos) {
    if(block) {
      childBlock = createBlock(line);
    }
    block = createBlock(line);
  }
  else if(line.compare("}") == 0) {
    if(childBlock) {
      block->addChildBlock(*childBlock);
      childBlock = NULL;
    } else {
      this->_blocks.push_back(*block);
      block = NULL;
    }
  }
  else {
    istringstream iss(line);
    vector<string> tokens;
    for (string s; iss >> s;) {
      tokens.push_back(s);
    }
    block->addProperty(tokens[0], vector<string>(tokens.begin() + 1, tokens.end()));
  }
}