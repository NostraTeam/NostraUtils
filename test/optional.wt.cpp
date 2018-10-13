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
    static nou::uint32 s_constructorCounter; // each construction this gets incremented
    static nou::uint32 s_destructorCounter;  // each destruction this gets incremented

    nou::uint32 m_value;

    Test1() : m_value(0)
    {
        s_constructorCounter++;
    }

    Test1(nou::uint32 value) : m_value(value)
    {
        s_constructorCounter++;
    }

    Test1(const Test1 &other) : m_value(other.m_value)
    {
        s_constructorCounter++;
    }

    Test1(Test1 &&other) : m_value(other.m_value)
    {
        s_constructorCounter++;
    }

    ~Test1() noexcept
    {
        s_destructorCounter++;
    }
};

class Test2
{
public:
    static nou::uint32 s_constructorCounter; // each construction this gets incremented
    static nou::uint32 s_destructorCounter;  // each destruction this gets incremented

    nou::uint32 m_value;

    Test2() : m_value(0)
    {
        s_constructorCounter++;
    }

    Test2(const Test1 &t1) : m_value(t1.m_value)
    {
        s_constructorCounter++;
    }

    Test2(const Test2 &other) : m_value(other.m_value)
    {
        s_constructorCounter++;
    }

    Test2(Test2 &&other) : m_value(other.m_value)
    {
        s_constructorCounter++;
    }

    ~Test2() noexcept
    {
        s_destructorCounter++;
    }
};

nou::uint32 Test1::s_constructorCounter = 0;
nou::uint32 Test1::s_destructorCounter  = 0;

nou::uint32 Test2::s_constructorCounter = 0;
nou::uint32 Test2::s_destructorCounter  = 0;

int main()
{
    NOU_TEST_IS_EQUAL(Test1::s_constructorCounter, 0); // check if properly initialized
    NOU_TEST_IS_EQUAL(Test1::s_destructorCounter, 0);  // check if no destructor called

    nou::Optional<Test1> opt1; // default constructor

    NOU_TEST_IS_EQUAL(Test1::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test1::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_TRUE(!opt1.isValid());

    {
        nou::boolean b = opt1;
        NOU_TEST_IS_EQUAL(opt1.isValid(), b);
    }

    nou::Optional<Test1> opt2 = nou::invalidOpt(); // construct form invalidOpt()

    NOU_TEST_IS_EQUAL(Test1::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test1::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_TRUE(!opt2.isValid());

    {
        nou::boolean b = opt2;
        NOU_TEST_IS_EQUAL(opt2.isValid(), b);
    }

    nou::Optional<Test1> opt3 = opt1; // copy; with uninitialized instance

    NOU_TEST_IS_EQUAL(Test1::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test1::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_TRUE(!opt3.isValid());

    {
        nou::boolean b = opt3;
        NOU_TEST_IS_EQUAL(opt3.isValid(), b);
    }

    nou::Optional<Test1> opt4{nou::Optional<Test1>()}; // move; with uninitialized instance

    NOU_TEST_IS_EQUAL(Test1::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test1::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_TRUE(!opt4.isValid());

    {
        nou::boolean b = opt4;
        NOU_TEST_IS_EQUAL(opt4.isValid(), b);
    }

    nou::Optional<Test2> opt5 = opt1; // copy; with uninitialized instance; other type

    NOU_TEST_IS_EQUAL(Test1::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test1::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_EQUAL(Test2::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test2::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_TRUE(!opt5.isValid());

    {
        nou::boolean b = opt5;
        NOU_TEST_IS_EQUAL(opt5.isValid(), b);
    }

    nou::Optional<Test2> opt6{nou::Optional<Test1>()}; // move; with uninitialized instance; other type

    NOU_TEST_IS_EQUAL(Test1::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test1::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_EQUAL(Test2::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test2::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_TRUE(!opt6.isValid());

    {
        nou::boolean b = opt6;
        NOU_TEST_IS_EQUAL(opt6.isValid(), b);
    }

    // put into block to trigger destruction
    {
        nou::Optional<Test1> opt7{Test1(5)}; // constructor from instance

        NOU_TEST_IS_TRUE(opt7.isValid());
        NOU_TEST_IS_EQUAL(opt7.get().m_value, 5);

        {
            nou::boolean b = opt7;
            NOU_TEST_IS_EQUAL(opt7.isValid(), b);
        }
    }

    NOU_TEST_IS_TRUE(Test1::s_constructorCounter > 0);
    NOU_TEST_IS_TRUE(Test1::s_destructorCounter > 0);

    Test1::s_constructorCounter = 0; // reset counter

    // put into block to trigger destruction
    {
        nou::Optional<Test1> opt{Test1(5)};
        nou::Optional<Test1> opt8{opt}; // copy; with initialized instance

        NOU_TEST_IS_TRUE(opt8.isValid());
        NOU_TEST_IS_EQUAL(opt8.get().m_value, 5);

        {
            nou::boolean b = opt8;
            NOU_TEST_IS_EQUAL(opt8.isValid(), b);
        }
    }

    NOU_TEST_IS_TRUE(Test1::s_constructorCounter > 0);
    NOU_TEST_IS_TRUE(Test1::s_destructorCounter > 1); // > 1, b/c the parameter object also gets destructed

    Test1::s_constructorCounter = 0; // reset counter

    nou::Optional<Test1> opt9{nou::Optional<Test1>(5)}; // move; with initialized instance

    NOU_TEST_IS_TRUE(Test1::s_constructorCounter > 0);
    NOU_TEST_IS_TRUE(Test1::s_destructorCounter > 1); // > 1, b/c the parameter object also gets destructed
    NOU_TEST_IS_TRUE(opt9.isValid());
    NOU_TEST_IS_EQUAL(opt9.get().m_value, 5);

    {
        nou::boolean b = opt9;
        NOU_TEST_IS_EQUAL(opt9.isValid(), b);
    }

    Test1::s_constructorCounter = 0; // reset counter
    Test1::s_destructorCounter  = 0; // reset counter
    Test2::s_constructorCounter = 0; // reset counter
    Test2::s_destructorCounter  = 0; // reset counter

    // put into block to trigger destruction
    {
        nou::Optional<Test1> opt{Test1(5)};
        nou::Optional<Test2> opt10{opt}; // copy; with initialized instance

        NOU_TEST_IS_TRUE(opt10.isValid());
        NOU_TEST_IS_EQUAL(opt10.get().m_value, 5);

        {
            nou::boolean b = opt10;
            NOU_TEST_IS_EQUAL(opt10.isValid(), b);
        }
    }

    NOU_TEST_IS_TRUE(Test1::s_constructorCounter == 2); // == 2, b/c of the objects in opt.
    NOU_TEST_IS_TRUE(Test1::s_destructorCounter == 2);  // == 2, b/c of the objects in opt.
    NOU_TEST_IS_TRUE(Test2::s_constructorCounter > 0);
    NOU_TEST_IS_TRUE(Test2::s_destructorCounter > 0);

    Test1::s_constructorCounter = 0; // reset counter
    Test2::s_destructorCounter  = 0; // reset counter
    Test2::s_constructorCounter = 0; // reset counter
    Test2::s_destructorCounter  = 0; // reset counter

    // put into block to trigger destruction
    {
        nou::Optional<Test2> opt11{nou::Optional<Test1>(5)}; // move; with initialized instance

        NOU_TEST_IS_EQUAL(opt11.get().m_value, 5);
        NOU_TEST_IS_TRUE(opt11.isValid());

        {
            nou::boolean b = opt11;
            NOU_TEST_IS_EQUAL(opt11.isValid(), b);
        }
    }

    NOU_TEST_IS_TRUE(Test1::s_constructorCounter > 0);
    NOU_TEST_IS_TRUE(Test1::s_destructorCounter > 0);
    NOU_TEST_IS_TRUE(Test2::s_constructorCounter > 0);
    NOU_TEST_IS_TRUE(Test2::s_destructorCounter > 0);
}
