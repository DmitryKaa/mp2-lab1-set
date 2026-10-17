#include <gtest.h>

TEST(Arithmetic, sum_of_first_five_odd_numbers_equals_five_squared)
{
  EXPECT_EQ(5 * 5, 1 + 3 + 5 + 7 + 9);
}

TEST(Arithmetic, sum_of_first_six_odd_numbers_equals_six_squared)
{
  EXPECT_EQ(6 * 6, 1 + 3 + 5 + 7 + 9 + 11);
}
