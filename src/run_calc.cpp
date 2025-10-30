#include <print>
#include "../include/calculator.h"

int run_calc(int /*argc*/, char** /*argv*/)
{
    Calculator calc;
    
    try 
    {
        std::print("add(10,4): {}\n", calc.add(10, 4));
        std::print("subtract(10,4): {}\n", calc.subtract(10, 4));
        std::print("multiply(10,4): {}\n", calc.multiply(10, 4));
        std::print("divide(10,4): {}\n", calc.divide(10, 4));
        std::print("modulus(10,4): {}\n", calc.modulus(10, 4));
    }
    catch (const std::exception& e)
    {
        std::println(stderr, "Calculator error: {}", e.what());
        return 1;
    }

    return 0;
}