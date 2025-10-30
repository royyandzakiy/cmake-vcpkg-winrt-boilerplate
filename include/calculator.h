#pragma once

//! A calculator class
/*!
  This class is a simple calculator class that can add, subtract, multiply, divide, and mod two numbers.
*/
class Calculator {
public:
    //! A member adding two arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param b an integer argument.
      \return the sum of a and b.
    */
    [[nodiscard]] int add(int a, int b) const;

    //! A member subtracting arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param b an integer argument.
      \return a minus b.
    */
    [[nodiscard]] int subtract(int a, int b) const;

    //! A member multiplying arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param b an integer argument.
      \return a * b.
    */    
    [[nodiscard]] int multiply(int a, int b) const;

    //! A member dividing arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param b an integer argument.
      \return a / b.
      \throws std::invalid_argument if b is zero
    */  
    [[nodiscard]] int divide(int a, int b) const;

    //! A member carrying out a modulus operation between arguments and returning an integer value.
    /*!
      \param a an integer argument.
      \param b an integer argument.
      \return a mod b.
      \throws std::invalid_argument if b is zero
    */ 
    [[nodiscard]] int modulus(int a, int b) const;
};