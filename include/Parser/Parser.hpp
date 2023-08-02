#pragma once
#include <map>
#include <sstream>
#include <vector>
#include "WebServerException/WebServerException.hpp"

enum TokenTypes {
  Http,
  Server,
  ServerName,
  Location,
  Listen,
  Root,
  IndexPage,
  ErrorPage,
  Include,
  If,
  Quotes,
  SquareBrackets,
  CurlyBrackets,
  Parenthesis,
  Semicolon,
  Return
};

typedef std::map<TokenTypes, std::string> Tokens;

class Token {
 public:
  Token(std::string value);
  Token();
  ~Token();
  bool isReservedWord();
  std::string getValue();

 private:
  std::string _value;
  int _symbolIndex;
};

class Parser {
 public:
  Parser();
  ~Parser();
  void parseLine(std::string line);
  std::vector<std::string> getSymbols();
  std::map<TokenTypes, Token> getTokens();

 private:
  std::vector<std::string> _symbols;
  std::map<TokenTypes, Token> _tokens;
  TokenTypes checkReservedWord(std::string word);
};