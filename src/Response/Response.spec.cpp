#include "Response/Response.hpp"

#include "gtest/gtest.h"

TEST(ResponseClass, StatusCodeTest) {
	Response res;

	// 100 Informational response
	// This is a success code that says the request was received and understood.
	res.setStatusCode(100);
	EXPECT_EQ(res.getStatusCode(), 100);

	// 200 OK
	// Whatever the request was trying to do was successful but is just it
	res.setStatusCode(200);
	EXPECT_EQ(res.getStatusCode(), 200);

	// 201 Created
	// This is a success code that says a resource was successfully created.
	res.setStatusCode(201);
	EXPECT_EQ(res.getStatusCode(), 201);

	// 202 Accepted
	// This is a success code that says the request was accepted but not yet
	// processed.
	res.setStatusCode(202);
	EXPECT_EQ(res.getStatusCode(), 202);

	// 204 No Content
	// This is a success code that says the request was successful but no
	//  content will be returned. Very common with DELETE requests.
	res.setStatusCode(204);
	EXPECT_EQ(res.getStatusCode(), 204);

	// 300 Redirection
	// This is a success code that says the requested resource has been moved
	res.setStatusCode(300);
	EXPECT_EQ(res.getStatusCode(), 300);

	// 301 Moved Permanently
	// Just says that the page at a certain URL has been permanently moved to a
	// new URL.
	res.setStatusCode(301);
	EXPECT_EQ(res.getStatusCode(), 301);

	// 302 Found
	// this status is used to tell the client that the page is at a new URL but
	// this is a temporary change
	res.setStatusCode(302);
	EXPECT_EQ(res.getStatusCode(), 302);

	// 304 Not Modified
	// This is used for caching and essentially just says that the resource
	// beign requested has not changed.
	res.setStatusCode(304);
	EXPECT_EQ(res.getStatusCode(), 304);

	// 400 Bad Request
	// This is a client error code that says the request was malformed.
	res.setStatusCode(400);
	EXPECT_EQ(res.getStatusCode(), 400);

	// 401 Unauthorized
	// This status code actually means you are unautheticated. (not logged)
	res.setStatusCode(401);
	EXPECT_EQ(res.getStatusCode(), 401);

	// 403 Forbidden
	// This status code means you are authenticated but not authorized to
	// perform this request.
	res.setStatusCode(403);
	EXPECT_EQ(res.getStatusCode(), 403);

	// 404 Not Found
	// This status code means the resource you are requesting does not exist.
	res.setStatusCode(404);
	EXPECT_EQ(res.getStatusCode(), 404);

	// 429 Too Many Requests
	// This status code means you have made too many requests and have been
	// rate limited.
	res.setStatusCode(429);
	EXPECT_EQ(res.getStatusCode(), 429);

	// 500 Internal Server Error
	// This status code means something went wrong on the server side.
	res.setStatusCode(500);
	EXPECT_EQ(res.getStatusCode(), 500);

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

