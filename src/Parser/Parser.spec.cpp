#include "Parser/Parser.hpp"
#include <gtest/gtest.h>

TEST(ParserTest, ShouldIdentifyAInvalidServerBlockDefinition) {
  Parser* p = new Parser();
  std::string buffer = "serverxpto {";

  EXPECT_THROW(
      {
        try {
          p->parseLine(buffer);
        } catch (std::exception& e) {
          EXPECT_STREQ("Invalid block definition serverxpto", e.what());
          throw;
        }
      },
      ParsingException);
}

TEST(ParserTest, ShouldThrowIfBlockDefinitionHasArguments) {
  Parser* p = new Parser();
  std::string buffer = "server xpto {";

  EXPECT_THROW(
      {
        try {
          p->parseLine(buffer);
        } catch (std::exception& e) {
          EXPECT_STREQ("Invalid number of arguments for a block", e.what());
          throw;
        }
      },
      ParsingException);
}

TEST(ParserTest, ShouldIdentifyAMissingCurlyBraces) {
  Parser* p = new Parser();
  std::string buffer = "server ";

  EXPECT_THROW(
      {
        try {
          p->parseLine(buffer);
        } catch (std::exception& e) {
          EXPECT_STREQ("Missing '{'", e.what());
          throw;
        }
      },
      ParsingException);
}

TEST(ParserTest, ShoulReturnTheFirstBlock) {
  Parser* p = new Parser();
  std::string buffer = "server {";
  p->parseLine(buffer);
  Block::iterator it = p->getNextServerBlock();
  EXPECT_EQ("server", it->first);
}

/* TEST(ParserTest, ShouldReadBlockContent) {
  Parser* p = new Parser();
  std::string buffer = "server {\n";
  buffer += "server_name xpto.com abcde.com\n";
  buffer += "listen 80";
  buffer += "error_page 404 /404.html\n";
  buffer += "client_max_body_size 100M\n";
} */

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}