#include "WebServerException.hpp"

#include <gtest/gtest.h>

TEST(ExceptionTest, WebServerExceptionMessage_Expect_Throw) {
  EXPECT_THROW(
      {
        try {
          throw WebServerException();
        } catch (std::exception &e) {
          EXPECT_STREQ("A WebServer exception occured", e.what());
          throw;
        }
      },
      WebServerException);
}

TEST(ExceptionTest, ConfigExceptionMessage_Expect_Throw) {
  EXPECT_THROW(
      {
        try {
          throw ConfigException();
        } catch (WebServerException &e) {
          EXPECT_STREQ("A config exception occured", e.what());
          throw;
        }
      },
      ConfigException);
}

void open_file(std::string filename) {
  if (filename != "server.conf") {
    throw ConfigException::FileNotFound(filename.c_str());
  }
}

TEST(ExceptionTest, ConfigExceptionFileNotFound_Expect_Throw) {
  EXPECT_THROW(
      {
        try {
          open_file("server.config");
        } catch (std::exception &e) {
          EXPECT_STREQ("File not found: server.config", e.what());
          throw;
        }
      },
      WebServerException);
}

TEST(ExceptionTest, ConfigExceptionFileNotFound_Expect_No_Throw) {
  EXPECT_NO_THROW({
    try {
      open_file("server.conf");
    } catch (std::exception &e) {
      EXPECT_STREQ("File not found: server.conf", e.what());
      throw;
    }
  });
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}