#include <queue>
#include <string>
#include <gtest/gtest.h>
using namespace std;

struct Student {
  string name;
  int age;
};

class StudentTest : public ::testing::Test {
 public:
    void SetUp() override {
      s1.name = "zhang";
      s1.age = 18;
      s2.name = "wang";
      s2.age = 18;
    }
    void TearDown() override {}
    Student s1;
    Student s2;

};

TEST_F(StudentTest, InitFunc) {
  EXPECT_EQ(s1.age, s2.age);
  EXPECT_STRNE(s1.name.c_str(), s2.name.c_str());
}

