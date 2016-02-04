#ifndef TEST_H_
#define TEST_H_
#include <iostream>
#include <string>
/// @brief Compare two values and print a summary of the comparison
/// @tparam T the type being tested (must have operator== defined)
/// @param name A string containing the name of this test comparison
/// @param expected A value of type T to be compared (the expected value)
/// @param actual A value of type T to be compared (the value being tested)
/// @return An integer, 0 if the test passed, 1 otherwise
template <typename T>
int test(const std::string & name, T expected, T actual)
{
  bool result = (expected == actual);
  std::string pass_string = result ? "Pass" : "FAIL";
  std::cout << "Testing [" << name << "]: Expected value is ["
            << expected << "], actual value is [" << actual
            << "] : " << pass_string << std::endl;
  return result ? 0 : 1;
}
/// @brief Compare two values and print a summary of the comparison
/// @tparam T the type being tested (must have operator== defined)
/// @param name A string containing the name of this test comparison
/// @param unexpected A value of type T to be compared (the value not expected, a fail value)
/// @param actual A value of type T to be compared (the value being tested)
/// @return An integer, 0 if the test passed, 1 otherwise
template <typename T>
int test_neq(const std::string & name, T unexpected, T actual)
{
  bool result = !(unexpected == actual);
  std::string pass_string = result ? "Pass" : "FAIL";
  std::cout << "Testing [" << name << "]: Unexpected value is ["
            << unexpected << "], actual value is [" << actual
            << "] : " << pass_string << std::endl;
  return result ? 0 : 1;
}
#endif
