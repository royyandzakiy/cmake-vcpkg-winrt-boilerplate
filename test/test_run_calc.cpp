#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include <iostream>
#include "../include/calculator.h"

extern int run_calc(int argc, char **argv);

class RunAppTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		// Redirect std::cout
		prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());
	}

	void TearDown() override
	{
		// Restore std::cout
		std::cout.rdbuf(prevcoutbuf);
	}

	std::stringstream buffer;
	std::streambuf *prevcoutbuf;
};

TEST_F(RunAppTest, OutputTest)
{
	run_calc(1, nullptr);
	std::string expected_output = "add(10,4): 14\nsub(10,4): 6\nmul(10,4): 40\ndiv(10,4): 2\nmod(10,4): 2\n";
	EXPECT_EQ(buffer.str(), expected_output);
}