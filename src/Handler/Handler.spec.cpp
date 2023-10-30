#include "Handler/Handler.hpp"

#include "gtest/gtest.h"

TEST(HandlerClass, ValidationStatusValidRequest) {
  const char bufferMock[] =
      "GET /teste.php HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  Request req(bufferMock);
  req.setValidationStatus(VALID_REQUEST);
  req.setServerRoot("www");
  Handler handler(&req);

  EXPECT_EQ(handler.getResponse().getStatusCode(), HTTP_OK);
  EXPECT_EQ(handler.getResponse().getReasonPhrase(), "OK");
}

TEST(HandlerClass, ValidationStatusInvalidHeader) {
  const char bufferMock[] =
      "GET /teste.php HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  const char expected[] = "<h1>Error 400</h1>";

  Request req(bufferMock);
  req.setValidationStatus(INVALID_HEADER);
  Handler handler(&req);

  EXPECT_EQ(handler.getResponse().getStatusCode(), HTTP_BAD_REQUEST);
  EXPECT_EQ(handler.getResponse().getReasonPhrase(), "Bad Request");
  EXPECT_EQ(handler.getResponse().getBody(), expected);
}

TEST(HandlerClass, Redirect) {
  const char bufferMock[] =
      "GET /teste.php HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  Request req(bufferMock);
  req.setValidationStatus(VALID_REQUEST);
  req.setRedirect("http://www.google.com/");
  Handler handler(&req);

  EXPECT_EQ(handler.getResponse().getStatusCode(), HTTP_MOVED);
  EXPECT_EQ(handler.getResponse().getReasonPhrase(), "Moved Permanently");
}

TEST(HandlerClass, ValidatePhpReturn) {
  const char bufferMock[] =
      "GET /teste.php HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  const char expected[] =
      "<!DOCTYPE html>\n"
      "<html lang=\"en\">\n"
      "<head>\n"
      "    <meta charset=\"UTF-8\">\n"
      "    <meta name=\"viewport\" content=\"width=device-width, "
      "initial-scale=1.0\">\n"
      "    <title>PHP - Hello, World!</title>\n"
      "</head>\n"
      "<body>\n"
      "        <h1>Hello, World!</h1>\n"
      "</body>\n"
      "</html>\n";

  Request req(bufferMock);
  req.setValidationStatus(VALID_REQUEST);
  req.setServerRoot("www");
  Handler handler(&req);

  EXPECT_EQ(handler.getResponse().getStatusCode(), HTTP_OK);
  EXPECT_EQ(handler.getResponse().getBody(), expected);
}

TEST(HandlerClass, ValidatePhpNotFoundReturn) {
  const char bufferMock[] =
      "GET /indexo.php HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  const char expected[] = "<h1>Error 404</h1>";

  Request req(bufferMock);
  req.setValidationStatus(VALID_REQUEST);
  req.setServerRoot("www");
  Handler handler(&req);

  EXPECT_EQ(handler.getResponse().getStatusCode(), HTTP_NOT_FOUND);
  EXPECT_EQ(handler.getResponse().getBody(), expected);
}

TEST(HandlerClass, ValidateHtmlReturn) {
  const char bufferMock[] =
      "GET /index.html HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  const char expected[] =
      "<!doctype html>"
      "<html>"
      "<head>"
      "    <title>This is the title of the webpage!</title>"
      "</head>"
      "<body>"
      "    <p>This is an example page.</p>"
      "</body>"
      "</html>";

  Request req(bufferMock);
  req.setValidationStatus(VALID_REQUEST);
  req.setServerRoot("www");
  Handler handler(&req);

  EXPECT_EQ(handler.getResponse().getStatusCode(), HTTP_OK);
  EXPECT_EQ(handler.getResponse().getBody(), expected);
}

TEST(HandlerClass, ValidateHtmlNotFoundReturn) {
  const char bufferMock[] =
      "GET /indexo.html HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  const char expected[] = "<h1>Error 404</h1>";

  Request req(bufferMock);
  req.setValidationStatus(VALID_REQUEST);
  req.setServerRoot("www");
  Handler handler(&req);

  EXPECT_EQ(handler.getResponse().getStatusCode(), HTTP_NOT_FOUND);
  EXPECT_EQ(handler.getResponse().getBody(), expected);
}

TEST(HandlerClass, ValidateFolderIndexFile) {
  const char bufferMock[] =
      "GET / HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  const char expected[] =
      "<!doctype html>"
      "<html>"
      "<head>"
      "    <title>This is the title of the webpage!</title>"
      "</head>"
      "<body>"
      "    <p>This is an example page.</p>"
      "</body>"
      "</html>";

  Request req(bufferMock);
  req.setValidationStatus(VALID_REQUEST);
  req.setServerRoot("www");
  std::vector<std::string> vIndex;
  vIndex.push_back("index.html");
  req.setIndex(vIndex);
  Handler handler(&req);

  EXPECT_EQ(handler.getResponse().getStatusCode(), HTTP_OK);
  EXPECT_EQ(handler.getResponse().getBody(), expected);
}

TEST(HandlerClass, ValidateFolderInvalidIndexFileAutoIndexOn) {
  const char bufferMock[] =
      "GET / HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  const char expected[] =
      "404.html\n"
      "teste.php\n"
      "index.html\n"
      "403.html\n"
      "400.html\n";

  Request req(bufferMock);
  req.setValidationStatus(VALID_REQUEST);
  req.setServerRoot("www");
  std::vector<std::string> vIndex;
  vIndex.push_back("index.htm");
  req.setIndex(vIndex);
  req.setAutoIndex("on");
  Handler handler(&req);

  EXPECT_EQ(handler.getResponse().getStatusCode(), HTTP_OK);
  EXPECT_EQ(handler.getResponse().getBody(), expected);
}

TEST(HandlerClass, ValidateFolderInvalidIndexFileAutoIndexOff) {
  const char bufferMock[] =
      "GET / HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  const char expected[] = "<h1>Error 404</h1>";

  Request req(bufferMock);
  req.setValidationStatus(VALID_REQUEST);
  req.setServerRoot("www");
  std::vector<std::string> vIndex;
  vIndex.push_back("index.htm");
  req.setIndex(vIndex);
  req.setAutoIndex("off");
  Handler handler(&req);

  EXPECT_EQ(handler.getResponse().getStatusCode(), HTTP_NOT_FOUND);
  EXPECT_EQ(handler.getResponse().getBody(), expected);
}

TEST(HandlerClass, ValidateInvalidFolder) {
  const char bufferMock[] =
      "GET /deno HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  const char expected[] = "<h1>Error 404</h1>";

  Request req(bufferMock);
  req.setValidationStatus(VALID_REQUEST);
  req.setServerRoot("www");
  req.setAutoIndex("off");
  Handler handler(&req);

  EXPECT_EQ(handler.getResponse().getStatusCode(), HTTP_NOT_FOUND);
  EXPECT_EQ(handler.getResponse().getBody(), expected);
}

TEST(HandlerClass, ValidateErroPage) {
  const char bufferMock[] =
      "GET /deno HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 0\r\n\r\n";

  const char expected[] = "<h1>Error 403</h1>";

  Request req(bufferMock);
  ErrorPage errPage;
  errPage.code = 404;
  errPage.path = "/403.html";
  std::vector<ErrorPage> vErrPage;
  vErrPage.push_back(errPage);
  req.setErrorPages(vErrPage);
  req.setValidationStatus(VALID_REQUEST);
  req.setServerRoot("www");
  req.setAutoIndex("off");
  Handler handler(&req);

  EXPECT_EQ(handler.getResponse().getStatusCode(), HTTP_NOT_FOUND);
  EXPECT_EQ(handler.getResponse().getBody(), expected);
}

TEST(HandlerClass, ValidateHandlePostResponse) {
  const char bufferMock[] =
      "POST /upload HTTP/1.1\r\n"
      "Host: localhost:3000\n"
      "Content-Type: text/plain\n"
      "Content-Length: 5\r\n\r\n"
      "Texto";

  Request req(bufferMock);
  req.setValidationStatus(VALID_REQUEST);
  Handler handler(&req);

  EXPECT_EQ(handler.getResponse().getStatusCode(), HTTP_CREATED);
  EXPECT_EQ(handler.getResponse().getReasonPhrase(), "CREATED");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}