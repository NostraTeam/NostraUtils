
/**
File:          pair.wt.cpp
Author:        Lukas Reichmann
Creation Date: 08.09.2018 (dd.mm.yyyy)

A test for pair.
*/

#ifndef NOU_TYPES_HPP
#    include "nostrautils/types.hpp"
#endif

#ifndef NOU_PAIR_HPP
#    include "nostrautils/pair.hpp"
#endif

#include <iostream>

#define NOU_TEST_IS_TRUE(...)                                \
    if(!(__VA_ARGS__))                                       \
    {                                                        \
        std::cout << "Failed in line: " << __LINE__ << "\n"; \
        exit(__LINE__);                                      \
    }

#define NOU_TEST_IS_EQUAL(a, b) NOU_TEST_IS_TRUE(((a) == (b)))

class Test
{
private:
    nou::int32 m_a;

public:
    Test(nou::int32 a) : m_a(a)
    {}

    nou::int32 get() const
    {
        return m_a;
    }
};

class NoCopy
{
public:
    NoCopy()               = default;
    NoCopy(const NoCopy &) = delete;
    NoCopy(NoCopy &&)      = default;
};

class NoMove
{
public:
    NoMove()               = default;
    NoMove(const NoMove &) = default;
    NoMove(NoMove &&)      = delete;
};

int main()
{
    nou::Pair<nou::int32, nou::float32> pair = nou::pair(5, 6.0f);

    NOU_TEST_IS_EQUAL(pair.first, 5);
    NOU_TEST_IS_EQUAL(pair.second, 6.0f);

    // test structured bindings
    auto [a, b] = pair;

    NOU_TEST_IS_EQUAL(a, pair.first);
    NOU_TEST_IS_EQUAL(b, pair.second);

    // test nou::pair()
    {
        Test test1(5);
        Test test2(6);
        nou::Pair<Test, Test> pair1 = nou::pair(test1, test2);

        NOU_TEST_IS_EQUAL(pair1.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair1.second.get(), 6);

        pair1 = nou::pair(Test(5), test2);

        NOU_TEST_IS_EQUAL(pair1.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair1.second.get(), 6);

        pair1 = nou::pair(test1, Test(6));

        NOU_TEST_IS_EQUAL(pair1.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair1.second.get(), 6);

        pair1 = nou::pair(Test(5), Test(6));

        NOU_TEST_IS_EQUAL(pair1.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair1.second.get(), 6);
    }

    // test copy constructors
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test, Test> pair1(test1, test2);
        nou::Pair<Test, Test> pair2(pair1);
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test, Test &> pair1(test1, test2);
        nou::Pair<Test, Test> pair2(pair1);
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test &, Test> pair1(test1, test2);
        nou::Pair<Test, Test> pair2(pair1);
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test &, Test &> pair1(test1, test2);
        nou::Pair<Test, Test> pair2(pair1);
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }

    // test move constructors
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test, Test> pair2(nou::pair<Test, Test>(test1, test2));
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test, Test> pair2(nou::pair<Test, Test &>(test1, test2));
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test, Test> pair2(nou::pair<Test &, Test>(test1, test2));
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test, Test> pair2(nou::pair<Test &, Test &>(test1, test2));
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }

    // test copy assign
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test, Test> pair1(test1, test2);
        nou::Pair<Test, Test> pair2(Test(1000), Test(2000));
        pair2 = pair1;
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test, Test &> pair1(test1, test2);
        nou::Pair<Test, Test> pair2(pair1);
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        // NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test &, Test> pair1(test1, test2);
        nou::Pair<Test, Test> pair2(Test(1000), Test(2000));
        pair2 = pair1;
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test &, Test &> pair1(test1, test2);
        nou::Pair<Test, Test> pair2(Test(1000), Test(2000));
        pair2 = pair1;
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }

    // test move assign
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test, Test> pair2(Test(1000), Test(2000));
        pair2 = nou::pair<Test, Test>(test1, test2);
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test, Test> pair2(Test(1000), Test(2000));
        pair2 = nou::pair<Test, Test &>(test1, test2);
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test, Test> pair2(Test(1000), Test(2000));
        pair2 = nou::pair<Test &, Test>(test1, test2);
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }
    {
        Test test1(5);
        Test test2(6);

        nou::Pair<Test, Test> pair2(Test(1000), Test(2000));
        pair2 = nou::pair<Test &, Test &>(test1, test2);
        NOU_TEST_IS_EQUAL(pair2.first.get(), 5);
        NOU_TEST_IS_EQUAL(pair2.second.get(), 6);
    }

    return 0;
}
