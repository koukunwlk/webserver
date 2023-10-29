#include "ConfigFileParser/ConfigFileParser.hpp"

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
    } catch (std::exception& e) {
      throw;
    }
  });

  delete p;
}

TEST(ParserTest, ShouldIdentifyAValidPropertyDefinition) {
  Parser* p = new Parser();
  std::string buffer = "server {\n";
  buffer += "server_name xpto.com abcde.com\n";
  buffer += "error_page 404 /404.html\n";
  buffer += "}\n";
  std::stringstream ss(buffer);
  std::string line;

  EXPECT_NO_THROW({
    try {
      while (std::getline(ss, line, '\n')) {
        p->parseLine(line);
      }
      std::vector<Block> blocks = p->getBlocks();
      Property property = blocks[0].getNextProperty();
      Property property2 = blocks[0].getNextProperty();

      EXPECT_EQ("server_name", property.first);
      EXPECT_EQ("xpto.com", property.second[0]);
      EXPECT_EQ("abcde.com", property.second[1]);
      EXPECT_EQ("error_page", property2.first);
      EXPECT_EQ("404", property2.second[0]);
    } catch (std::exception& e) {
      throw;
    }
  });

  delete p;
}

TEST(ParserTest, ShouldIdentifyAValidConfigFile) {
  Parser* p = new Parser();
  std::string buffer;
  buffer += "server {\n";
  buffer += "server_name xpto.com abcde.com\n";
  buffer += "listen 80\n";
  buffer += "error_page 404 /404.html\n";
  buffer += "client_max_body_size 100M\n";
  buffer += "location / {\n";
  buffer += "root /var/www/html;\n";
  buffer += "index index.html index.htm index.nginx-debian.html;\n";
  buffer += "allow GET; # permite o metodo GET\n";
  buffer += "return 301 /path/to/new/resource;\n";
  buffer += "autoindex off # desabilita a listagem de arquivos\n";
  buffer += "try_files $uri $uri /index.html; \n";
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
      Property property = blocks[0].getNextProperty();
      Property property2 = blocks[0].getNextProperty();
      Property property3 = blocks[0].getNextProperty();
      Property property4 = blocks[0].getNextProperty();
      Property childBlockProperty = childBlocks[0].getNextProperty();
      Property childBlockProperty2 = childBlocks[0].getNextProperty();
      Property childBlockProperty3 = childBlocks[0].getNextProperty();
      Property childBlockProperty4 = childBlocks[0].getNextProperty();
      Property childBlockProperty5 = childBlocks[0].getNextProperty();
      Property childBlockProperty6 = childBlocks[0].getNextProperty();

      EXPECT_EQ("server_name", property.first);
      EXPECT_EQ("listen", property2.first);
      EXPECT_EQ("error_page", property3.first);
      EXPECT_EQ("client_max_body_size", property4.first);
      EXPECT_EQ("root", childBlockProperty.first);
      EXPECT_EQ("index", childBlockProperty2.first);
      EXPECT_EQ("allow", childBlockProperty3.first);
      EXPECT_EQ("return", childBlockProperty4.first);
      EXPECT_EQ("autoindex", childBlockProperty5.first);
      EXPECT_EQ("try_files", childBlockProperty6.first);
      EXPECT_EQ("location", childBlocks[0].getName());
      EXPECT_EQ("server", blocks[0].getName());

    } catch (std::exception& e) {
      throw;
    }
  });

  delete p;
}

TEST(ParserTest, ShouldIdentifyAValidConfigFileWithMultipleLocationsBlock) {
  Parser* p = new Parser();
  std::string buffer;
  buffer += "server {\n";
  buffer += "server_name xpto.com abcde.com\n";
  buffer += "listen 80\n";
  buffer += "error_page 404 /404.html\n";
  buffer += "client_max_body_size 100M\n";
  buffer += "location / {\n";
  buffer += "root /var/www/html;\n";
  buffer += "index index.html index.htm index.nginx-debian.html;\n";
  buffer += "allow GET; # permite o metodo GET\n";
  buffer += "return 301 /path/to/new/resource;\n";
  buffer += "autoindex off # desabilita a listagem de arquivos\n";
  buffer += "try_files $uri $uri /index.html; \n";
  buffer += "}\n";
  buffer += "location / {\n";
  buffer += "root /var/www/html;\n";
  buffer += "index index.html index.htm index.nginx-debian.html;\n";
  buffer += "allow GET; # permite o metodo GET\n";
  buffer += "return 301 /path/to/new/resource;\n";
  buffer += "autoindex off # desabilita a listagem de arquivos\n";
  buffer += "try_files $uri $uri /index.html; \n";
  buffer += "}\n";
  buffer += "location / {\n";
  buffer += "root /var/www/html;\n";
  buffer += "index index.html index.htm index.nginx-debian.html;\n";
  buffer += "allow GET; # permite o metodo GET\n";
  buffer += "return 301 /path/to/new/resource;\n";
  buffer += "autoindex off # desabilita a listagem de arquivos\n";
  buffer += "try_files $uri $uri /index.html; \n";
  buffer += "}\n";
  buffer += " \t}\n";

  std::stringstream ss(buffer);
  std::string line;

  EXPECT_NO_THROW({
    try {
      while (std::getline(ss, line, '\n')) {
        p->parseLine(line);
      }
      std::vector<Block> blocks = p->getBlocks();
      std::vector<Block> childBlocks = blocks[0].getChildBlocks();
      Block childBlock = childBlocks[0];
      Block childBlock2 = childBlocks[1];
      Block childBlock3 = childBlocks[2];

      EXPECT_EQ("location", childBlocks[0].getName());
      EXPECT_EQ("location", childBlocks[1].getName());
      EXPECT_EQ("location", childBlocks[2].getName());

    } catch (std::exception& e) {
      throw;
    }
  });

  delete p;
}

TEST(ParserTest, ShouldIdentifyAValidConfigFileWithMultipleServerBlocks) {
  Parser* p = new Parser();
  std::string buffer;
  buffer += "server { # isso e um server\n";
  buffer += "server_name xpto.com abcde.com\n";
  buffer += "listen 80\n";
  buffer += "error_page 404 /404.html\n";
  buffer += "client_max_body_size 100M\n";
  buffer += "location / {\n";
  buffer += "root /var/www/html;\n";
  buffer += "index index.html index.htm index.nginx-debian.html;\n";
  buffer += "allow GET; # permite o metodo GET\n";
  buffer += "return 301 /path/to/new/resource;\n";
  buffer += "autoindex off # desabilita a listagem de arquivos\n";
  buffer += "try_files $uri $uri /index.html; \n";
  buffer += "}\n";
  buffer += "}\n";
  buffer += "server {\n";
  buffer += "server_name xpto.com abcde.com\n";
  buffer += "listen 80\n";
  buffer += "error_page 404 /404.html\n";
  buffer += "client_max_body_size 100M\n";
  buffer += "location / {\n";
  buffer += "root /var/www/html;\n";
  buffer += "index index.html index.htm index.nginx-debian.html;\n";
  buffer += "allow GET; # permite o metodo GET\n";
  buffer += "return 301 /path/to/new/resource;\n";
  buffer += "autoindex off # desabilita a listagem de arquivos\n";
  buffer += "try_files $uri $uri /index.html; \n";
  buffer += "}\n";
  buffer += "}\n";
  buffer += "server {\n";
  buffer += "server_name xpto.com abcde.com\n";
  buffer += "listen 80\n";
  buffer += "error_page 404 /404.html\n";
  buffer += "client_max_body_size 100M\n";
  buffer += "location / {\n";
  buffer += "root /var/www/html;\n";
  buffer += "index index.html index.htm index.nginx-debian.html;\n";
  buffer += "allow GET; # permite o metodo GET\n";
  buffer += "return 301 /path/to/new/resource;\n";
  buffer += "autoindex off # desabilita a listagem de arquivos\n";
  buffer += "try_files $uri $uri /index.html; \n";
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

      EXPECT_EQ("server", blocks[0].getName());
      EXPECT_EQ("server", blocks[1].getName());
      EXPECT_EQ("server", blocks[2].getName());

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