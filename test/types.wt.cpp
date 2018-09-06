
/**
File:          cmakebuild.wt.cpp
Author:        Lukas Reichmann
Creation Date: 04.09.2018 (dd.mm.yyyy)

A test that test the types component. Since said component does merely provide types and not any logic that
can be executed, this test only tests if the size of the single types match their intended size.
*/

#ifndef NOU_TYPES_HPP
#include "nostrautils/types.hpp"
#endif

#include <cstdlib>
#include <iostream>

#define NOU_TEST_IS_TRUE(...) if(!(__VA_ARGS__)) {std::cout << "Failed in line: " << __LINE__ << "\n"; \
	exit(__LINE__);}

#define NOU_TEST_IS_EQUAL(a, b) NOU_TEST_IS_TRUE((a == b))

template<typename T, nou::int32 SIZE>
nou::boolean testFloatingPoint()
{
	nou::uint32 smallest = sizeof(float);
	nou::uint32 largest = sizeof(long double);

	if (sizeof(T) == SIZE) //sizes match
		return true;
	else if (SIZE < smallest && sizeof(T) == smallest) //desired size is smaller than the smallest possible
		return true;
	else if (SIZE > largest && sizeof(T) == largest) //desired size is larger than the largest possible
		return true;
	else
		return false;
}

int main()
{
	NOU_TEST_IS_EQUAL(sizeof(nou::byte), 1);

	//testing the size of this types does not make sense as it only stores two values, size does not matter
	//NOU_TEST_IS_EQUAL(sizeof(nou::boolean), 1);

	NOU_TEST_IS_EQUAL(sizeof(nou::char8), 1);

	NOU_TEST_IS_EQUAL(sizeof(nou::char16), 2);

	NOU_TEST_IS_EQUAL(sizeof(nou::char32), 4);

	NOU_TEST_IS_EQUAL(sizeof(nou::int8), 1);

	NOU_TEST_IS_EQUAL(sizeof(nou::int16), 2);

	NOU_TEST_IS_EQUAL(sizeof(nou::int32), 4);

	NOU_TEST_IS_EQUAL(sizeof(nou::int64), 8);

	NOU_TEST_IS_EQUAL(sizeof(nou::uint8), 1);

	NOU_TEST_IS_EQUAL(sizeof(nou::uint16), 2);

	NOU_TEST_IS_EQUAL(sizeof(nou::uint32), 4);

	NOU_TEST_IS_EQUAL(sizeof(nou::uint64), 8);

	NOU_TEST_IS_EQUAL(sizeof(nou::uint32), 4);

	NOU_TEST_IS_EQUAL(sizeof(nou::uint64), 8);

	NOU_TEST_IS_EQUAL(sizeof(nou::sizeType), sizeof(void*));

	NOU_TEST_IS_TRUE(testFloatingPoint<nou::float32, 4>());

	NOU_TEST_IS_TRUE(testFloatingPoint<nou::float64, 8>());
}