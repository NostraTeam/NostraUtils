
/**
File:          declval.wt.cpp
Author:        Lukas Reichmann
Creation Date: 26.09.2018 (dd.mm.yyyy)

A test for declval.
*/

// TODO: add test that checks that the program does not compile when the returned value is actually used
// TODO: use nou meta functions

#ifndef NOU_TYPES_HPP
#    include "nostrautils/types.hpp"
#endif

#ifndef NOU_DECLVAL_HPP
#    include "nostrautils/declval.hpp"
#endif

#include <cstdlib>
#include <iostream>
#include <type_traits>

#define NOU_TEST_IS_TRUE(...)                                \
    if(!(__VA_ARGS__))                                       \
    {                                                        \
        std::cout << "Failed in line: " << __LINE__ << "\n"; \
        exit(__LINE__);                                      \
    }

#define NOU_TEST_IS_EQUAL(a, b) NOU_TEST_IS_TRUE(((a) == (b)))

int main()
{
    NOU_TEST_IS_TRUE(std::is_same<decltype(nou::declval<nou::int32>()), nou::int32 &&>::value);
    NOU_TEST_IS_TRUE(std::is_same<decltype(nou::declval<void>()), void>::value);
}
