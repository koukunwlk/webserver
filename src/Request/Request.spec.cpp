#include "Request/Request.hpp"

#include <gtest/gtest.h>

TEST(RequestClass, Parse) {
  const char bufferMock[] =
      "GET /teste.php HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  Request req(bufferMock);

  EXPECT_EQ(req.rawData, std::string(bufferMock));
  EXPECT_EQ(req.header.method, std::string("GET"));
  EXPECT_EQ(req.header.target, std::string("/teste.php"));
  EXPECT_EQ(req.header.protocol, std::string("HTTP/1.1"));
  EXPECT_EQ(req.header.host, std::string("localhost:3000"));
  EXPECT_EQ(req.header.contentType, std::string("text/plain"));
  EXPECT_EQ(req.header.contentLength, 0);
}

TEST(RequestClass, Validate) {
  const char bufferMock[] =
      "PATCH /teste.php HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  EXPECT_THROW(
      {
        try {
          Request req(bufferMock);
        } catch (std::exception &e) {
          EXPECT_STREQ("A Request Validation exception occured", e.what());
          throw;
        }
      },
      RequestValidationException);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}