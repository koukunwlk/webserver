#include "Parser/Parser.hpp"
using namespace std;

Parser::Parser() { this->currentBlock = this->_blocks.begin(); }

Parser::~Parser() {}

void Parser::parseLine(string line) {
  istringstream iss(line);
  if (isBlock(line)) {
    return;
  }
}

bool Parser::isBlock(string line) {
  if (line.find_first_of("{") != string::npos ||
      line.find_first_of("server") != string::npos ||
      line.find_first_of("location") != string::npos) {
    validateBlockDefinition(line);
    return true;
  }
  return false;
}

bool Parser::validateBlockDefinition(string line) throw(ParsingException) {
  vector<string> validBlocks;
  istringstream iss(line);
  string currentToken;
  vector<string> tokens;
  vector<string>::iterator it;
  vector<string> blockTokens;

  validBlocks.push_back("server");
  validBlocks.push_back("location");

  while (iss >> currentToken) {
    tokens.push_back(currentToken);
  }

  if (tokens.back().compare("{") != 0) {
    throw ParsingException("Missing '{'");
  }

  if (tokens.size() != 2) {
    throw ParsingException("Invalid number of arguments for a block");
  }

  tokens.pop_back();
  currentToken = tokens.back();
  it = find(validBlocks.begin(), validBlocks.end(), currentToken);

  if (!(it != validBlocks.end())) {
    string errorMessage = "Invalid block definition ";
    errorMessage += currentToken;
    throw ParsingException(errorMessage);
  }
  this->_blocks.insert(make_pair(currentToken, blockTokens));

  return true;
}

Block::iterator Parser::getNextServerBlock() { return this->currentBlock++; }