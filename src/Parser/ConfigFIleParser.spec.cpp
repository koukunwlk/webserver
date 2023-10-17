#include "ConfigFIleParser/ConfigFIleParser.hpp"
#include <gtest/gtest.h>

TEST(ParserTest, ShouldIdentifyAInvalidServerBlockDefinition) {
  Parser* p = new Parser();
  std::string buffer = "server abc {";

  EXPECT_THROW(
      {
        try {
          p->parseLine(buffer);
        } catch (std::exception& e) {
          EXPECT_STREQ("Invalid block definition 'server abc {'", e.what());
          throw;
        }
      },
      ParsingException);

  delete p;
}

TEST(ParserTest, ShouldIdentifyAValidServerBlockDefinition) {
  Parser* p = new Parser();
  std::string buffer = "server {";

  EXPECT_NO_THROW({
    try {
      p->parseLine(buffer);
    } catch (std::exception& e) {
      throw;
    }
  });
  delete p;
}

TEST(ParserTest, ShouldIdentifyAValidLocationBlockDefinition) {
  Parser* p = new Parser();
  std::string buffer = "location ~ {";

  EXPECT_NO_THROW({
    try {
      p->parseLine(buffer);
    } catch (std::exception& e) {
      throw;
    }
  });
  delete p;
}

TEST(ParserTest, ShouldIdentifyAValidChildBlockDefinition) {
  Parser* p = new Parser();
  std::string buffer = "server {\n";
  buffer += "location ~ {\n";
  buffer += "}\n";
  buffer += "}\n";
  std::stringstream ss(buffer);
  std::string line;

  EXPECT_NO_THROW({
    try {
      while (std::getline(ss, line, '\n')) {
        p->parseLine(line);
      }
      std::vector<Block> blocks = p->getBlocks();

      std::vector<Block> childBlocks = blocks[0].getChildBlocks();

      EXPECT_EQ("location", childBlocks[0].getName());
      EXPECT_EQ(1, 1);
    } catch (std::exception& e) {
      throw;
    }
  });

  delete p;
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