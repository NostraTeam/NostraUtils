/**
File:          optional.wt.cpp
Author:        Lukas Reichmann
Creation Date: 28.09.2018 (dd.mm.yyyy)

A test for optional.
*/

#ifndef NOU_OPTIONAL_HPP
#    include "nostrautils/optional.hpp"
#endif

#include <cstdlib>
#include <cstring>
#include <iostream>

#define NOU_TEST_IS_TRUE(...)                                \
    if(!(__VA_ARGS__))                                       \
    {                                                        \
        std::cout << "Failed in line: " << __LINE__ << "\n"; \
        exit(__LINE__);                                      \
    }

#define NOU_TEST_IS_EQUAL(a, b) NOU_TEST_IS_TRUE(((a) == (b)))

class Test1
{
public:
    static nou::uint32 s_counter; // each construction this gets incremented

    nou::uint32 m_value;

    Test1() : m_value(0)
    {
        s_counter++;
    }

    Test1(nou::uint32 value) : m_value(value)
    {
        s_counter++;
    }

    Test1(const Test1 &other) : m_value(other.m_value)
    {
        s_counter++;
    }

    Test1(Test1 &&other) : m_value(other.m_value)
    {
        s_counter++;
    }
};

class Test2
{
public:
    static nou::uint32 s_counter; // each construction this gets incremented

    nou::uint32 m_value;

    Test2() : m_value(0)
    {
        s_counter++;
    }

    Test2(const Test1 &t1) : m_value(t1.m_value)
    {
        s_counter++;
    }

    Test2(const Test2 &other) : m_value(other.m_value)
    {
        s_counter++;
    }

    Test2(Test2 &&other) : m_value(other.m_value)
    {
        s_counter++;
    }
};

nou::uint32 Test1::s_counter = 0;
nou::uint32 Test2::s_counter = 0;

int main()
{
    nou::uint32 counterShould = 0;
    NOU_TEST_IS_EQUAL(Test1::s_counter, 0); // check if properly initialized

    nou::Optional<Test1> opt1; // default constructor

    NOU_TEST_IS_EQUAL(Test1::s_counter, 0); // must not be constructed yet
    NOU_TEST_IS_TRUE(!opt1.isValid());

    nou::Optional<Test1> opt2 = nou::invalidOpt(); // construct form invalidOpt()

    NOU_TEST_IS_EQUAL(Test1::s_counter, 0); // must not be constructed yet
    NOU_TEST_IS_TRUE(!opt2.isValid());

    nou::Optional<Test1> opt3 = opt1; // copy; with uninitialized instance

    NOU_TEST_IS_EQUAL(Test1::s_counter, 0); // must not be constructed yet
    NOU_TEST_IS_TRUE(!opt3.isValid());

    nou::Optional<Test1> opt4{nou::Optional<Test1>()}; // move; with uninitialized instance

    NOU_TEST_IS_EQUAL(Test1::s_counter, 0); // must not be constructed yet
    NOU_TEST_IS_TRUE(!opt4.isValid());

    nou::Optional<Test2> opt5 = opt1; // copy; with uninitialized instance; other type

    NOU_TEST_IS_EQUAL(Test1::s_counter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test2::s_counter, 0); // must not be constructed yet
    NOU_TEST_IS_TRUE(!opt5.isValid());

    nou::Optional<Test2> opt6{nou::Optional<Test1>()}; // move; with uninitialized instance; other type

    NOU_TEST_IS_EQUAL(Test1::s_counter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test2::s_counter, 0); // must not be constructed yet
    NOU_TEST_IS_TRUE(!opt6.isValid());

    nou::Optional<Test1> opt7{Test1(5)}; // constructor from instance

    NOU_TEST_IS_TRUE(Test1::s_counter > 0);
    NOU_TEST_IS_TRUE(opt7.isValid());
    NOU_TEST_IS_EQUAL(opt7.get().m_value, 5);

    Test1::s_counter = 0; // reset counter

    nou::Optional<Test1> opt8{opt7}; // copy; with initialized instance

    NOU_TEST_IS_TRUE(Test1::s_counter > 0);
    NOU_TEST_IS_TRUE(opt8.isValid());
    NOU_TEST_IS_EQUAL(opt8.get().m_value, 5);

    Test1::s_counter = 0; // reset counter

    nou::Optional<Test1> opt9{nou::Optional<Test1>(5)}; // move; with initialized instance

    NOU_TEST_IS_TRUE(Test1::s_counter > 0);
    NOU_TEST_IS_TRUE(opt9.isValid());
    NOU_TEST_IS_EQUAL(opt9.get().m_value, 5);

    Test1::s_counter = 0; // reset counter
    Test2::s_counter = 0; // reset counter

    nou::Optional<Test2> opt10{opt7}; // copy; with initialized instance

    NOU_TEST_IS_TRUE(Test1::s_counter == 0);
    NOU_TEST_IS_TRUE(Test2::s_counter > 0);
    NOU_TEST_IS_TRUE(opt10.isValid());
    NOU_TEST_IS_EQUAL(opt10.get().m_value, 5);

    Test1::s_counter = 0; // reset counter
    Test2::s_counter = 0; // reset counter

    nou::Optional<Test2> opt11{nou::Optional<Test1>(5)}; // move; with initialized instance

    NOU_TEST_IS_TRUE(Test1::s_counter > 0);
    NOU_TEST_IS_TRUE(Test2::s_counter > 0);
    NOU_TEST_IS_EQUAL(opt11.get().m_value, 5);
    NOU_TEST_IS_TRUE(opt11.isValid());

    // TODO: constexpr
}
