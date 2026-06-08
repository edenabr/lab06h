#include <solver.h>

#include <gtest/gtest.h>

TEST(Solver, QuadraticEquation)
{
  float x1 = 0;
  float x2 = 0;

  solve(1, 2, 1, x1, x2);

  EXPECT_FLOAT_EQ(x1, -1);
  EXPECT_FLOAT_EQ(x2, -1);
}
