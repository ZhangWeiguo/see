#include "gtest/gtest.h"

TEST(TESTA, TESTA_A) { EXPECT_EQ(12, 12); }
int main(int args, char** argv) {
  testing::InitGoogleTest();  //初始化GoogleTest
  return RUN_ALL_TESTS();
}