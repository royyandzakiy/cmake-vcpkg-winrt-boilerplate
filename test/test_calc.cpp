#include <gtest/gtest.h>
#include "../include/calculator.h"

class CalculatorTest : public ::testing::Test
{
protected:
	Calculator calc;
};

TEST_F(CalculatorTest, Add)
{
	EXPECT_EQ(calc.add(2, 3), 5);
	EXPECT_EQ(calc.add(-2, 3), 1);
	EXPECT_EQ(calc.add(0, 0), 0);
}

TEST_F(CalculatorTest, Sub)
{
	EXPECT_EQ(calc.sub(5, 3), 2);
	EXPECT_EQ(calc.sub(3, 5), -2);
	EXPECT_EQ(calc.sub(0, 0), 0);
}

TEST_F(CalculatorTest, Mul)
{
	EXPECT_EQ(calc.mul(2, 3), 6);
	EXPECT_EQ(calc.mul(-2, 3), -6);
	EXPECT_EQ(calc.mul(0, 5), 0);
}

TEST_F(CalculatorTest, Div)
{
	EXPECT_EQ(calc.div(6, 3), 2);
	EXPECT_EQ(calc.div(5, 2), 2);
}

TEST_F(CalculatorTest, Mod)
{
	EXPECT_EQ(calc.mod(6, 4), 2);
	EXPECT_EQ(calc.mod(5, 2), 1);
}