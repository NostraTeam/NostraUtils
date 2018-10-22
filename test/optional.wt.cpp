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

class TriviallyDestructible2;

class TriviallyDestructible
{
public:
    nou::uint32 m_value;

    constexpr TriviallyDestructible() : m_value(0)
    {}

    constexpr TriviallyDestructible(const TriviallyDestructible2 &other) : m_value(0)
    {}

    constexpr TriviallyDestructible(nou::uint32 value) : m_value(value)
    {}

    constexpr TriviallyDestructible(const TriviallyDestructible &other) : m_value(other.m_value)
    {}

    constexpr TriviallyDestructible(TriviallyDestructible &&other) : m_value(other.m_value)
    {}
};

class TriviallyDestructible2
{
public:
    nou::uint32 m_value;

    constexpr TriviallyDestructible2() : m_value(0)
    {}

    constexpr TriviallyDestructible2(const TriviallyDestructible &other) : m_value(other.m_value)
    {}

    constexpr TriviallyDestructible2(nou::uint32 value) : m_value(value)
    {}

    constexpr TriviallyDestructible2(const TriviallyDestructible2 &other) : m_value(other.m_value)
    {}

    constexpr TriviallyDestructible2(TriviallyDestructible2 &&other) : m_value(other.m_value)
    {}
};

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

    ~Test1()
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

    ~Test2()
    {
        s_destructorCounter++;
    }
};

nou::uint32 Test1::s_constructorCounter = 0;
nou::uint32 Test1::s_destructorCounter  = 0;

nou::uint32 Test2::s_constructorCounter = 0;
nou::uint32 Test2::s_destructorCounter  = 0;

void normalTests()
{
    NOU_TEST_IS_EQUAL(Test1::s_constructorCounter, 0); // check if properly initialized
    NOU_TEST_IS_EQUAL(Test1::s_destructorCounter, 0);  // check if no destructor called

    nou::Optional<Test1> opt1; // default constructor

    NOU_TEST_IS_EQUAL(Test1::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test1::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_TRUE(!opt1.isValid());

    nou::Optional<Test1> opt2 = nou::invalidOpt(); // construct form invalidOpt()

    NOU_TEST_IS_EQUAL(Test1::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test1::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_TRUE(!opt2.isValid());

    nou::Optional<Test1> opt3 = opt1; // copy; with uninitialized instance

    NOU_TEST_IS_EQUAL(Test1::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test1::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_TRUE(!opt3.isValid());

    nou::Optional<Test1> opt4{nou::Optional<Test1>()}; // move; with uninitialized instance

    NOU_TEST_IS_EQUAL(Test1::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test1::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_TRUE(!opt4.isValid());

    nou::Optional<Test2> opt5 = opt1; // copy; with uninitialized instance; other type

    NOU_TEST_IS_EQUAL(Test1::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test1::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_EQUAL(Test2::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test2::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_TRUE(!opt5.isValid());

    nou::Optional<Test2> opt6{nou::Optional<Test1>()}; // move; with uninitialized instance; other type

    NOU_TEST_IS_EQUAL(Test1::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test1::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_EQUAL(Test2::s_constructorCounter, 0); // must not be constructed yet
    NOU_TEST_IS_EQUAL(Test2::s_destructorCounter, 0);  // check if no destructor called
    NOU_TEST_IS_TRUE(!opt6.isValid());

    // put into block to trigger destruction
    {
        nou::Optional<Test1> opt7{Test1(5)}; // constructor from instance

        NOU_TEST_IS_TRUE(opt7.isValid());
        NOU_TEST_IS_EQUAL(opt7.get().m_value, 5);
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
    }

    NOU_TEST_IS_TRUE(Test1::s_constructorCounter > 0);
    NOU_TEST_IS_TRUE(Test1::s_destructorCounter > 1); // > 1, b/c the parameter object also gets destructed

    Test1::s_constructorCounter = 0; // reset counter

    nou::Optional<Test1> opt9{nou::Optional<Test1>(5)}; // move; with initialized instance

    NOU_TEST_IS_TRUE(Test1::s_constructorCounter > 0);
    NOU_TEST_IS_TRUE(Test1::s_destructorCounter > 1); // > 1, b/c the parameter object also gets destructed
    NOU_TEST_IS_TRUE(opt9.isValid());
    NOU_TEST_IS_EQUAL(opt9.get().m_value, 5);

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
    }

    NOU_TEST_IS_TRUE(Test1::s_constructorCounter > 0);
    NOU_TEST_IS_TRUE(Test1::s_destructorCounter > 0);
    NOU_TEST_IS_TRUE(Test2::s_constructorCounter > 0);
    NOU_TEST_IS_TRUE(Test2::s_destructorCounter > 0);

    nou::Optional<Test2> opt12;

    Test2 t1{15};

    NOU_TEST_IS_TRUE(opt12.getOr(t1).m_value == 15);
    NOU_TEST_IS_TRUE(opt12.ptr() == nullptr);

    opt12.set(Test2{10});

    NOU_TEST_IS_TRUE(opt12.get().m_value == 10);
    NOU_TEST_IS_TRUE(opt12.get().m_value == (*opt12).m_value);
    NOU_TEST_IS_TRUE(opt12.get().m_value == opt12->m_value);
    NOU_TEST_IS_TRUE(opt12.getOr(t1).m_value == opt12.get().m_value);
    NOU_TEST_IS_TRUE(opt12.ptr()->m_value == opt12.get().m_value);
    NOU_TEST_IS_TRUE(opt12.ptr() == opt12.operator->());

    nou::Optional<Test2> opt13;

    Test2 t2 = opt13.moveOr(Test2{15});

    NOU_TEST_IS_TRUE(t2.m_value == 15);

    opt13.set(Test2{10});

    Test2 t3 = opt13.move();

    NOU_TEST_IS_TRUE(t3.m_value == 10);

    nou::Optional<Test2> opt14;

    opt14.set(Test2{10});

    Test2 t4 = opt14.moveOr(Test2{15});

    NOU_TEST_IS_TRUE(t4.m_value == 10);

    nou::Optional<Test2> opt15;
    nou::Optional<Test2> opt16;

    opt15.set(Test2{10});
    opt16 = Test2{10};

    NOU_TEST_IS_TRUE(opt15->m_value == opt16->m_value);
}

void constexprTests()
{
    constexpr nou::Optional<TriviallyDestructible> trivDest0;
    static_assert(trivDest0.isValid() == false);

    constexpr nou::Optional<TriviallyDestructible> trivDest1(5);
    static_assert(trivDest1.isValid() == true);
    static_assert(trivDest1.get().m_value == 5);
    static_assert(trivDest1.get().m_value == (*trivDest1).m_value);

    constexpr nou::Optional<TriviallyDestructible2> trivDest2 = trivDest0;
    static_assert(trivDest2.isValid() == false);

    constexpr nou::Optional<TriviallyDestructible2> trivDest3 = trivDest1;
    static_assert(trivDest3.isValid() == true);
    static_assert(trivDest3.get().m_value == 5);

    constexpr nou::Optional<TriviallyDestructible2> trivDest4 = trivDest2;
    static_assert(trivDest4.isValid() == false);

    constexpr nou::Optional<TriviallyDestructible2> trivDest5 = trivDest3;
    static_assert(trivDest5.isValid() == true);
    static_assert(trivDest5.get().m_value == 5);

    constexpr nou::Optional<TriviallyDestructible2> trivDest6 = nou::invalidOpt();
    static_assert(trivDest6.isValid() == false);

    static_assert(trivDest5.getOr(TriviallyDestructible2{10}).m_value == 5);  // trivDest6 is valid
    static_assert(trivDest6.getOr(TriviallyDestructible2{10}).m_value == 10); // trivDest5 is invalid
}

int main()
{
    normalTests();
    constexprTests();
}
