#include "../include/calculator.h"
#include <stdexcept>

int Calculator::add(int a, int b) const { return a + b; }

int Calculator::subtract(int a, int b) const { return a - b; }

int Calculator::multiply(int a, int b) const { return a * b; }

int Calculator::divide(int a, int b) const 
{ 
    if (b == 0) 
    {
        throw std::invalid_argument("Division by zero");
    }
    return a / b; 
}

int Calculator::modulus(int a, int b) const 
{ 
    if (b == 0) 
    {
        throw std::invalid_argument("Modulus by zero");
    }
    return a % b; 
}