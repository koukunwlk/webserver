#include "Parser/Parser.hpp"
#include <gtest/gtest.h>

TEST(ParserTest, ShouldSplitLineInSymbols) {
  // Arrange
  std::string line = "this is a exemple line\n";
  std::stringstream symbols = std::stringstream(line);
  std::string symbol;
  Parser newParser;
  std::vector<std::string> expectedSymbols;
  while (symbols >> symbol) expectedSymbols.push_back(symbol);

  // Act
  newParser.parseLine(line);
  std::vector<std::string> receivedSymbols = newParser.getSymbols();

  // Assert
  for (size_t i = 0; i < expectedSymbols.size(); i++) {
    ASSERT_EQ(expectedSymbols[i], receivedSymbols[i]);
  }
}

TEST(ParserTest, ShouldGenerateToken) {
  // Arrange
  std::string line = "this is a exemple line\n";
  std::stringstream symbols = std::stringstream(line);
  std::string symbol;
  Parser newParser;

  // Act
  newParser.parseLine(line);
  std::map<TokenTypes, Token> tokens = newParser.getTokens();

  ASSERT_EQ(tokens[0].first, TokenTypes[identifier]);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}