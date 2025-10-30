#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include "../include/calculator.h"

extern int run_calc(int argc, char** argv);

class RunAppTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Redirect std::cout
        prev_cout_buf = std::cout.rdbuf(buffer.rdbuf());
    }

    void TearDown() override {
        // Restore std::cout
        std::cout.rdbuf(prev_cout_buf);
    }

    std::stringstream buffer;
    std::streambuf* prev_cout_buf;
};

TEST_F(RunAppTest, OutputTest) {
    run_calc(1, nullptr);
    
    const std::string expected_output = 
        "add(10,4): 14\n"
        "subtract(10,4): 6\n"
        "multiply(10,4): 40\n"
        "divide(10,4): 2\n"
        "modulus(10,4): 2\n";
    
    EXPECT_EQ(buffer.str(), expected_output);
}