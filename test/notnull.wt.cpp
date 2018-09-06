
/**
File:          notnull.wt.cpp
Author:        Lukas Reichmann
Creation Date: 06.09.2018 (dd.mm.yyyy)

A test for notnull. Tests all of the functions in the component that are testable as a compilable test (there 
are some parts in the component that should trigger a compile error, this is tested in another test).
*/

#ifndef NOU_NOTNULL_HPP
#include "nostrautils/notnull.hpp"
#endif

#include <cstdlib>
#include <iostream>

#define NOU_TEST_IS_TRUE(...) if(!(__VA_ARGS__)) {std::cout << "Failed in line: " << __LINE__ << "\n"; \
	exit(__LINE__);}

#define NOU_TEST_IS_EQUAL(a, b) NOU_TEST_IS_TRUE((a == b))

class Test
{
private:
	nou::int32 m_a;

public:
	constexpr Test(nou::int32 a) :
		m_a(a)
	{}

	constexpr nou::int32 getA() const
	{
		return m_a;
	}
};

int main()
{
	nou::int32 i1 = 5;
	nou::int32 i2 = 10;
	nou::NotNull<nou::int32*> notNull1 = &i1;

	NOU_TEST_IS_EQUAL(*notNull1, i1);

	notNull1 = &i2;
	NOU_TEST_IS_EQUAL(*notNull1, i2);

	Test test1(5);
	Test test2(10);

	nou::NotNull<Test*> notNull2 = &test1;
	NOU_TEST_IS_EQUAL(notNull2->getA(), test1.getA());

	notNull2 = &test2;
	NOU_TEST_IS_EQUAL(notNull2->getA(), test2.getA());



	static constexpr nou::int32 i3 = 5;
	constexpr nou::NotNull<const nou::int32*> notNull3 = &i3;

	NOU_TEST_IS_EQUAL(*notNull3, i3);

	static constexpr Test test3(5);

	constexpr nou::NotNull<const Test*> notNull4 = &test3;
	NOU_TEST_IS_EQUAL(notNull4->getA(), test1.getA());



	return 0;
}