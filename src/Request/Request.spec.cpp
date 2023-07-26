#include "Request/Request.hpp"

#include <gtest/gtest.h>

TEST(RequestClass, HeaderParse) {
  const char bufferMock[] =
      "GET /teste.php HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  Request req(bufferMock);

  EXPECT_EQ(req.rawData, bufferMock);
  EXPECT_EQ(req.header.method, "GET");
  EXPECT_EQ(req.header.target, "/teste.php");
  EXPECT_EQ(req.header.protocol, "HTTP/1.1");
  EXPECT_EQ(req.header.host, "localhost:3000");
  EXPECT_EQ(req.header.contentType, "text/plain");
  EXPECT_EQ(req.header.contentLength, 0);
}

TEST(RequestClass, HeaderValidateMethodException) {
  const char bufferMock[] =
      "PATCH / HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  EXPECT_THROW(
      {
        try {
          Request req(bufferMock);
        } catch (std::exception &e) {
          EXPECT_STREQ("Invalid Method: PATCH", e.what());
          throw;
        }
      },
      RequestValidationException::InvalidMethod);
}

TEST(RequestClass, HeaderValidateContentLengthException) {
  const char bufferMock[] =
      "GET / HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: -1\r\n\r\n";

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

TEST(RequestClass, BodyParse) {
  const char bufferMock[] =
      "POST /teste.php HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 10\r\n\r\n"
      "SampleBody";

  Request req(bufferMock);

  EXPECT_EQ(req.body, "SampleBody");
}

TEST(RequestClass, BodyContentLengthException) {
  const char bufferMock[] =
      "POST /teste.php HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 9\r\n\r\n"
      "SampleBody";

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