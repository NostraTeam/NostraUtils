
/**
File:          notnull.wt.cpp
Author:        Lukas Reichmann
Creation Date: 06.09.2018 (dd.mm.yyyy)

A test for notnull. Tests all of the functions in the component that are testable as a compilable test (there 
are some parts in the component that should trigger a compile error, this is tested in another test).
*/

#ifndef NOU_TYPES_HPP
#include "nostrautils/types.hpp"
#endif

#ifndef NOU_NOTNULL_HPP
#include "nostrautils/notnull.hpp"
#endif

#include <cstdlib>
#include <iostream>
#include <string>

#define NOU_TEST_IS_TRUE(...) if(!(__VA_ARGS__)) {std::cout << "Failed in line: " << __LINE__ << "\n"; \
	exit(__LINE__);}

#define NOU_TEST_IS_EQUAL(a, b) NOU_TEST_IS_TRUE(((a) == (b)))

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
	{
		nou::int32 i1 = 5;
		nou::int32 i2 = 10;
		nou::int32 i3[] = { 1, 2, 3, 4, 5 };
		const nou::int32 i4 = 15;
		nou::NotNull<nou::int32*> notNull1 = &i1;

		//rawPtr()
		NOU_TEST_IS_EQUAL(notNull1.rawPtr(), &i1);

		notNull1 = &i2;

		//rawPtr() after assign to check assign
		NOU_TEST_IS_EQUAL(notNull1.rawPtr(), &i2);

		//indirection
		NOU_TEST_IS_EQUAL(*notNull1, i2);

		//subscript
		notNull1 = i3;
		NOU_TEST_IS_EQUAL(notNull1[0], i3[0]);
		NOU_TEST_IS_EQUAL(notNull1[1], i3[1]);
		NOU_TEST_IS_EQUAL(notNull1[2], i3[2]);
		NOU_TEST_IS_EQUAL(notNull1[3], i3[3]);

		//comparison
		notNull1 = &i1;
		const nou::NotNull<nou::int32*> notNull2 = &i2;

		NOU_TEST_IS_EQUAL(notNull1 == notNull1, true);
		NOU_TEST_IS_EQUAL(notNull1 == notNull1, true);
		NOU_TEST_IS_EQUAL(notNull1 != notNull1, false);
		NOU_TEST_IS_EQUAL(notNull1 != notNull1, false);
		NOU_TEST_IS_EQUAL(notNull1 <  notNull1, false);
		NOU_TEST_IS_EQUAL(notNull1 <  notNull1, false);
		NOU_TEST_IS_EQUAL(notNull1 >  notNull1, false);
		NOU_TEST_IS_EQUAL(notNull1 >  notNull1, false);
		NOU_TEST_IS_EQUAL(notNull1 <= notNull1, true);
		NOU_TEST_IS_EQUAL(notNull1 <= notNull1, true);
		NOU_TEST_IS_EQUAL(notNull1 >= notNull1, true);
		NOU_TEST_IS_EQUAL(notNull1 >= notNull1, true);

		NOU_TEST_IS_EQUAL(notNull1 == notNull2, &i1 == &i2);
		NOU_TEST_IS_EQUAL(notNull2 == notNull1, &i2 == &i1);
		NOU_TEST_IS_EQUAL(notNull1 != notNull2, &i1 != &i2);
		NOU_TEST_IS_EQUAL(notNull2 != notNull1, &i2 != &i1);
		NOU_TEST_IS_EQUAL(notNull1 <  notNull2, &i1 <  &i2);
		NOU_TEST_IS_EQUAL(notNull2 >  notNull1, &i2 >  &i1);
		NOU_TEST_IS_EQUAL(notNull1 >  notNull2, &i1 >  &i2);
		NOU_TEST_IS_EQUAL(notNull2 >  notNull1, &i2 >  &i1);
		NOU_TEST_IS_EQUAL(notNull1 <= notNull2, &i1 <= &i2);
		NOU_TEST_IS_EQUAL(notNull2 <= notNull1, &i2 <= &i1);
		NOU_TEST_IS_EQUAL(notNull1 >= notNull2, &i1 >= &i2);
		NOU_TEST_IS_EQUAL(notNull2 >= notNull1, &i2 >= &i1);

		NOU_TEST_IS_EQUAL(notNull1 == &i4,      &i1 == &i4);
		NOU_TEST_IS_EQUAL(&i4      == notNull1, &i4 == &i1);
		NOU_TEST_IS_EQUAL(notNull1 != &i4,      &i1 != &i4);
		NOU_TEST_IS_EQUAL(&i4      != notNull1, &i4 != &i1);
		NOU_TEST_IS_EQUAL(notNull1 < &i4,       &i1 <  &i4);
		NOU_TEST_IS_EQUAL(&i4      < notNull1,  &i4 <  &i1);
		NOU_TEST_IS_EQUAL(notNull1 > &i4,       &i1 >  &i4);
		NOU_TEST_IS_EQUAL(&i4      > notNull1,  &i4 >  &i1);
		NOU_TEST_IS_EQUAL(notNull1 <= &i4,      &i1 <= &i4);
		NOU_TEST_IS_EQUAL(&i4      <= notNull1, &i4 <= &i1);
		NOU_TEST_IS_EQUAL(notNull1 >= &i4,      &i1 >= &i4);
		NOU_TEST_IS_EQUAL(&i4      >= notNull1, &i4 >= &i1);
		//comparison end

		//logical start
		NOU_TEST_IS_EQUAL(notNull1, true);
		NOU_TEST_IS_EQUAL(!notNull1, false);

		NOU_TEST_IS_EQUAL(notNull1 && notNull2, true);
		NOU_TEST_IS_EQUAL(notNull1 && &i1,      true);
		NOU_TEST_IS_EQUAL(notNull1 && nullptr,  false);
		NOU_TEST_IS_EQUAL(notNull2 && notNull1, true);
		NOU_TEST_IS_EQUAL(&i1      && notNull1, true);
		NOU_TEST_IS_EQUAL(nullptr  && notNull1, false);
		NOU_TEST_IS_EQUAL(notNull1 || notNull2, true);
		NOU_TEST_IS_EQUAL(notNull1 || &i1,      true);
		NOU_TEST_IS_EQUAL(notNull1 || nullptr,  true);
		NOU_TEST_IS_EQUAL(notNull2 || notNull1, true);
		NOU_TEST_IS_EQUAL(&i1      || notNull1, true);
		NOU_TEST_IS_EQUAL(nullptr  || notNull1, true);
		//logical end

		//increment/decrement start
		notNull1 = i3;
		nou::int32 *i3Ptr = i3;

		NOU_TEST_IS_EQUAL((notNull1++).rawPtr(), i3Ptr++);
		NOU_TEST_IS_EQUAL((notNull1--).rawPtr(), i3Ptr--);
		NOU_TEST_IS_EQUAL((++notNull1).rawPtr(), ++i3Ptr);
		NOU_TEST_IS_EQUAL((--notNull1).rawPtr(), --i3Ptr);
		//increment/decrement start

		//arithmetic start
		notNull1 = &i1;

		NOU_TEST_IS_EQUAL(notNull1 +       10, &i1      + 10);
		NOU_TEST_IS_EQUAL(10       + notNull1, 10       + &i1);
		NOU_TEST_IS_EQUAL(notNull1 - 10,       &i1      - 10);
		NOU_TEST_IS_EQUAL(notNull1 - notNull1, notNull1 - notNull1);
		NOU_TEST_IS_EQUAL(notNull1 - notNull1, notNull1 - notNull1);
		NOU_TEST_IS_EQUAL(notNull1 - notNull2, notNull1 - notNull2);
		NOU_TEST_IS_EQUAL(notNull2 - notNull1, notNull2 - notNull1);
		NOU_TEST_IS_EQUAL(notNull1 - &i1,      notNull1 - &i1);
		NOU_TEST_IS_EQUAL(notNull1 - &i1,      notNull1 - &i1);
		NOU_TEST_IS_EQUAL(notNull1 - &i2,      notNull1 - &i2);
		//arithmetic end

		//assign start
		nou::int32 *i1Cpy = &i1;

		NOU_TEST_IS_EQUAL(notNull1 += 10, i1Cpy += 10);
		NOU_TEST_IS_EQUAL(notNull1 -= 10, i1Cpy -= 10);
		//assign end

		Test test1(5);

		//arrow
		nou::NotNull<Test*> notNull3(&test1);
		NOU_TEST_IS_EQUAL(notNull3->getA(), test1.getA());
	}

	//constexpr part
	{
		static constexpr nou::int32 i1 = 5;
		static constexpr nou::int32 i2 = 10;
		static constexpr nou::int32 i3[] = { 1, 2, 3, 4, 5 };
		static constexpr nou::int32 i4 = 15;
		constexpr nou::NotNull<const nou::int32*> notNull1 = &i1;

		//rawPtr()
		NOU_TEST_IS_EQUAL(notNull1.rawPtr(), &i1);

		//indirection
		NOU_TEST_IS_EQUAL(*notNull1, i1);

		//subscript
		constexpr nou::NotNull<const nou::int32*> notNull4 = i3;

		NOU_TEST_IS_EQUAL(notNull4[0], i3[0]);
		NOU_TEST_IS_EQUAL(notNull4[1], i3[1]);
		NOU_TEST_IS_EQUAL(notNull4[2], i3[2]);
		NOU_TEST_IS_EQUAL(notNull4[3], i3[3]);

		//comparison
		const nou::NotNull<const nou::int32*> notNull2 = &i2;

		NOU_TEST_IS_EQUAL(notNull1 == notNull1, true);
		NOU_TEST_IS_EQUAL(notNull1 == notNull1, true);
		NOU_TEST_IS_EQUAL(notNull1 != notNull1, false);
		NOU_TEST_IS_EQUAL(notNull1 != notNull1, false);
		NOU_TEST_IS_EQUAL(notNull1 <  notNull1, false);
		NOU_TEST_IS_EQUAL(notNull1 <  notNull1, false);
		NOU_TEST_IS_EQUAL(notNull1 >  notNull1, false);
		NOU_TEST_IS_EQUAL(notNull1 >  notNull1, false);
		NOU_TEST_IS_EQUAL(notNull1 <= notNull1, true);
		NOU_TEST_IS_EQUAL(notNull1 <= notNull1, true);
		NOU_TEST_IS_EQUAL(notNull1 >= notNull1, true);
		NOU_TEST_IS_EQUAL(notNull1 >= notNull1, true);

		NOU_TEST_IS_EQUAL(notNull1 == notNull2, &i1 == &i2);
		NOU_TEST_IS_EQUAL(notNull2 == notNull1, &i2 == &i1);
		NOU_TEST_IS_EQUAL(notNull1 != notNull2, &i1 != &i2);
		NOU_TEST_IS_EQUAL(notNull2 != notNull1, &i2 != &i1);
		NOU_TEST_IS_EQUAL(notNull1 <  notNull2, &i1 <  &i2);
		NOU_TEST_IS_EQUAL(notNull2 >  notNull1, &i2 >  &i1);
		NOU_TEST_IS_EQUAL(notNull1 >  notNull2, &i1 >  &i2);
		NOU_TEST_IS_EQUAL(notNull2 >  notNull1, &i2 >  &i1);
		NOU_TEST_IS_EQUAL(notNull1 <= notNull2, &i1 <= &i2);
		NOU_TEST_IS_EQUAL(notNull2 <= notNull1, &i2 <= &i1);
		NOU_TEST_IS_EQUAL(notNull1 >= notNull2, &i1 >= &i2);
		NOU_TEST_IS_EQUAL(notNull2 >= notNull1, &i2 >= &i1);

		NOU_TEST_IS_EQUAL(notNull1 == &i4, &i1 == &i4);
		NOU_TEST_IS_EQUAL(&i4      == notNull1, &i4 == &i1);
		NOU_TEST_IS_EQUAL(notNull1 != &i4, &i1 != &i4);
		NOU_TEST_IS_EQUAL(&i4      != notNull1, &i4 != &i1);
		NOU_TEST_IS_EQUAL(notNull1 < &i4, &i1 <  &i4);
		NOU_TEST_IS_EQUAL(&i4      < notNull1, &i4 <  &i1);
		NOU_TEST_IS_EQUAL(notNull1 > &i4, &i1 >  &i4);
		NOU_TEST_IS_EQUAL(&i4      > notNull1, &i4 >  &i1);
		NOU_TEST_IS_EQUAL(notNull1 <= &i4, &i1 <= &i4);
		NOU_TEST_IS_EQUAL(&i4      <= notNull1, &i4 <= &i1);
		NOU_TEST_IS_EQUAL(notNull1 >= &i4, &i1 >= &i4);
		NOU_TEST_IS_EQUAL(&i4      >= notNull1, &i4 >= &i1);
		//comparison end

		//logical start
		NOU_TEST_IS_EQUAL(notNull1, true);
		NOU_TEST_IS_EQUAL(!notNull1, false);

		NOU_TEST_IS_EQUAL(notNull1 && notNull2, true);
		NOU_TEST_IS_EQUAL(notNull1 && &i1, true);
		NOU_TEST_IS_EQUAL(notNull1 && nullptr, false);
		NOU_TEST_IS_EQUAL(notNull2 && notNull1, true);
		NOU_TEST_IS_EQUAL(&i1      && notNull1, true);
		NOU_TEST_IS_EQUAL(nullptr  && notNull1, false);
		NOU_TEST_IS_EQUAL(notNull1 || notNull2, true);
		NOU_TEST_IS_EQUAL(notNull1 || &i1, true);
		NOU_TEST_IS_EQUAL(notNull1 || nullptr, true);
		NOU_TEST_IS_EQUAL(notNull2 || notNull1, true);
		NOU_TEST_IS_EQUAL(&i1      || notNull1, true);
		NOU_TEST_IS_EQUAL(nullptr  || notNull1, true);
		//logical end
		
		//arithmetic start
		NOU_TEST_IS_EQUAL(notNull1 +       10, &i1      + 10);
		NOU_TEST_IS_EQUAL(10       + notNull1, 10       + &i1);
		NOU_TEST_IS_EQUAL(notNull1 - 10,       &i1      - 10);
		NOU_TEST_IS_EQUAL(notNull1 - notNull1, notNull1 - notNull1);
		NOU_TEST_IS_EQUAL(notNull1 - notNull1, notNull1 - notNull1);
		NOU_TEST_IS_EQUAL(notNull1 - notNull2, notNull1 - notNull2);
		NOU_TEST_IS_EQUAL(notNull2 - notNull1, notNull2 - notNull1);
		NOU_TEST_IS_EQUAL(notNull1 - &i1,      notNull1 - &i1);
		NOU_TEST_IS_EQUAL(notNull1 - &i1,      notNull1 - &i1);
		NOU_TEST_IS_EQUAL(notNull1 - &i2,      notNull1 - &i2);
		//arithmetic end

		static constexpr Test test1(5);

		//arrow
		nou::NotNull<const Test*> notNull3 = &test1;
		NOU_TEST_IS_EQUAL(notNull3->getA(), test1.getA());
	}

	return 0;
}