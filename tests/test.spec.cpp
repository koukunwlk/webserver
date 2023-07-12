#include <gtest/gtest.h>

#include "../include/webserver.hpp"
// #include "tests.hpp"

using std::string;

string actualValTrue = "hello gtest";
string actualValFalse = "hello world";
string expectVal = "hello gtest";

TEST(StringComparisonTest, StrEqual) { EXPECT_EQ(expectVal, actualValTrue); }

TEST(StringComparisonTest, StrNotEqual) {
  EXPECT_NE(expectVal, actualValFalse);
}

TEST(StringComparisonTest, StrEqualFalse) {
  EXPECT_EQ(expectVal, actualValFalse);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}