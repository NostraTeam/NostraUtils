
/**
File:          declval.wt.cpp
Author:        Lukas Reichmann
Creation Date: 26.09.2018 (dd.mm.yyyy)

A test for declval.
*/

// TODO: add test that checks that the program does not compile when the returned value of nou::declval() is
// actually used

#ifndef NOU_TYPES_HPP
#    include "nostrautils/types.hpp"
#endif

#ifndef NOU_UTIL_HPP
#    include "nostrautils/util.hpp"
#endif

#ifndef NOU_META_HPP
#    include "nostrautils/meta.hpp"
#endif

#include <cstdlib>
#include <iostream>
#include <type_traits>
#include <utility>

#define NOU_TEST_IS_TRUE(...)                                \
    if(!(__VA_ARGS__))                                       \
    {                                                        \
        std::cout << "Failed in line: " << __LINE__ << "\n"; \
        exit(__LINE__);                                      \
    }

#define NOU_TEST_IS_EQUAL(a, b) NOU_TEST_IS_TRUE(((a) == (b)))

class Test
{
public:
    nou::boolean m_wasCopied;
    nou::boolean m_wasMoved;

    Test() : m_wasCopied(false), m_wasMoved(false)
    {}

    Test(const Test &other) : m_wasCopied(true), m_wasMoved(false)
    {}

    Test(Test &&other) : m_wasCopied(false), m_wasMoved(true)
    {}
};

template<typename T>
void test(T &&t)
{
    Test test1 = nou::forward<Test>(t);

    Test test2 = std::forward<Test>(t);

    // nou::forward() is hard to test on its own, compare with std::forward()
    NOU_TEST_IS_EQUAL(test1.m_wasCopied, test2.m_wasCopied);
    NOU_TEST_IS_EQUAL(test1.m_wasMoved, test2.m_wasMoved);
}

int main()
{
    NOU_TEST_IS_TRUE(nou::AreSame<decltype(nou::declval<nou::int32>()), nou::int32 &&>::value);
    NOU_TEST_IS_TRUE(nou::AreSame<decltype(nou::declval<void>()), void>::value);

    Test t1;

    Test t2 = t1;
    Test t3 = nou::move(t1);

    NOU_TEST_IS_TRUE(t2.m_wasCopied);
    NOU_TEST_IS_TRUE(!t2.m_wasMoved);

    NOU_TEST_IS_TRUE(!t3.m_wasCopied);
    NOU_TEST_IS_TRUE(t3.m_wasMoved);

    Test t;

    test(t);      // pass l-value reference
    test(Test()); // pass r-value reference
}
