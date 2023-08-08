#include "Response/Response.hpp"

#include "gtest/gtest.h"

// Status Code Tests
TEST(ResponseClass, StatusCodeTest) {
  Response res;

  // 100 Informational response
  // This is a success code that says the request was received and understood.
  res.setStatusCode(100);
  EXPECT_EQ(res.getStatusCode(), 100);
}

TEST(ResponseClass, checkProtocolVersionTest) {
  Response res;

  res.setProtocolVersion("HTTP/1.1");
  EXPECT_EQ(res.getProtocolVersion(), "HTTP/1.1");
}

TEST(ResponseClass, checkReasonPhraseTest) {
  Response res;

  res.setReasonPhrase("OK");
  EXPECT_EQ(res.getReasonPhrase(), "OK");
}

// RESPONSE HEADER FIELDS TEST

TEST(ResponseClass, checkContentTypeTest) {
  Response res;

  res.setContentType("text/html; charset=utf-8");
  EXPECT_EQ(res.getContentType(), "text/html; charset=utf-8");
}

TEST(ResponseClass, checkContentLengthTest) {
  Response res;

  res.setContentLength(1024);
  EXPECT_EQ(res.getContentLength(), 1024);
}

TEST(ResponseClass, checkBodyTest) {
  Response res;

  res.setBody("Hello World!");
  EXPECT_EQ(res.getBody(), "Hello World!");
}

TEST(ResponseClass, checkConstructorTest) {
  Response res(200, "text/html; charset=utf-8", "Hello World!");

  EXPECT_EQ(res.getProtocolVersion(), "HTTP/1.1");
  EXPECT_EQ(res.getStatusCode(), 200);
  EXPECT_EQ(res.getReasonPhrase(), "OK");
  EXPECT_EQ(res.getContentType(), "text/html; charset=utf-8");
  EXPECT_EQ(res.getBody(), "Hello World!");
}

TEST(ResponseClass, checkOstreamOperatorOverloadTest) {
  Response res(200, "text/html; charset=utf-8", "Hello World!");

  std::string responseHeader =
      "HTTP/1.1 200 OK\r\nContent-Type: text/html; "
      "charset=utf-8\r\nContent-Length: 12\r\n\r\nHello World!";

  std::stringstream ostreamResult;

  ostreamResult << res;

  EXPECT_EQ(ostreamResult.str(), responseHeader);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
