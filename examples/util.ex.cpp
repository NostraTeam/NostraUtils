
/**
File:          util.ex.cpp
Author:        Lukas Reichmann
Creation Date: 28.09.2018 (dd.mm.yyyy)

An example on how to use the declval component.
*/

#ifndef NOU_TYPES_HPP
#    include "nostrautils/types.hpp"
#endif

#ifndef NOU_UTIL_HPP
#    include "nostrautils/util.hpp"
#endif

#include <iostream>
#include <typeinfo>

template<typename T>
void print()
{
    // prints the type name that is the result of adding an \ilc{nou::int32} to an instance of \ilc{T} without
    // ever constructing an instance of the latter type.
    std::cout << typeid(decltype(nou::int32() + nou::declval<T>())).name() << "\n";
}

class Test
{
public:
    nou::boolean m_wasCopied;
    nou::boolean m_wasMoved;

    Test() : m_wasCopied(false), m_wasMoved(false){};

    Test(const Test &other) : m_wasCopied(true), m_wasMoved(false)
    {
        std::cout << "Test::Test(const Test&)\n";
    }

    Test(Test &&other) : m_wasCopied(false), m_wasMoved(true)
    {
        std::cout << "Test::Test(Test&&)\n";
    }
};

void moveTest()
{
    Test t1;

    Test t2 = t1;            // print: Test::Test(const Test&)
    Test t3 = nou::move(t1); // print: Test::Test(Test&&)
}

template<typename T>
void forwardTest(T &&t)
{
    Test test = nou::forward<Test>(t);

    std::cout << "Copied: " << test.m_wasCopied << "; Moved: " << test.m_wasMoved << "\n";
}

int main()
{
    // an example for nou::declval()
    print<nou::float32>();

    // an example for nou::move()
    moveTest();

    Test test;

    forwardTest(test);   // passing l-value reference
    forwardTest(Test()); // passing r-value reference
}
