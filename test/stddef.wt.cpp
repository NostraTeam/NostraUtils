
/**
File:          stddef.wt.cpp
Author:        Lukas Reichmann
Creation Date: 04.09.2018 (dd.mm.yyyy)

A test for stddef. Tests most of the macros in that component to the degree that they are testable. Some 
macros are not being tested at all because they are not testable.
*/

#ifndef NOU_STDDEF_HPP
#include "nostrautils/stddef.hpp"
#endif

#include <cstdlib>
#include <cstring>
#include <iostream>

#define NOU_TEST_IS_TRUE(...) if(!(__VA_ARGS__)) {std::cout << "Failed in line: " << __LINE__ << "\n"; \
	exit(__LINE__);}

#define NOU_TEST_IS_EQUAL(a, b) NOU_TEST_IS_TRUE(((a) == (b)))

int main()
{
#if defined __MINGW32__
	NOU_TEST_IS_EQUAL(NOU_OS, NOU_OS_MINGW);
#elif defined __CYGWIN__
	NOU_TEST_IS_EQUAL(NOU_OS, NOU_OS_CYGWIN);
#elif defined _WIN32 //must be after MinGW and Cygwin
	NOU_TEST_IS_EQUAL(NOU_OS, NOU_OS_WINDOWS);
#elif defined __linux__
	NOU_TEST_IS_EQUAL(NOU_OS, NOU_OS_LINUX);
#endif

#ifdef __clang__
	NOU_TEST_IS_EQUAL(NOU_COMPILER, NOU_COMPILER_CLANG);
#elif defined _MSC_VER
	NOU_TEST_IS_EQUAL(NOU_COMPILER, NOU_COMPILER_MSVC);
#elif defined __GNUC__
	NOU_TEST_IS_EQUAL(NOU_COMPILER, NOU_COMPILER_GCC);
#endif

	NOU_TEST_IS_TRUE(std::strcmp(NOU_STRINGIFY(sometext), "sometext") == 0);

	//WARNING: If any lines are added above, the right parameter of the next test case needs to be adjusted
	NOU_TEST_IS_TRUE(std::strcmp(NOU_LINE_STRING, "47") == 0);

	return 0;
}