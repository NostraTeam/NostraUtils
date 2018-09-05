
/**
File:          assert.wt.cpp
Author:        Lukas Reichmann
Creation Date: 05.09.2018 (dd.mm.yyyy)

A test for stddef. Tests most of the macros in that component to the degree that they are testable. Some
macros are not being tested at all because they are not testable.
*/

#ifndef NOU_ASSERT_HPP
#include "nostrautils/assert.hpp"
#endif

#include <cstdlib>
#include <cstring>

#define NOU_TEST_IS_TRUE(...) if(!(__VA_ARGS__)) {exit(__LINE__);}

#define NOU_TEST_IS_EQUAL(a, b) NOU_TEST_IS_TRUE((a == b))

void assertCallbackSucceed(const nou::char8 *msg, const nou::char8 *fnName, nou::uint32 line,
	const nou::char8 *file)
{
	//this callback is only active if the assertion should succeed in the test; it is an error if it gets 
	//called
	NOU_TEST_IS_TRUE(false);
}

void assertCallbackFail(const nou::char8 *msg, const nou::char8 *fnName, nou::uint32 line,
	const nou::char8 *file)
{
	NOU_TEST_IS_TRUE(std::strcmp(fnName, NOU_FUNC_NAME));
	NOU_TEST_IS_EQUAL(line, 56);
	NOU_TEST_IS_TRUE(std::strcmp(file, __FILE__));
}

int main()
{
	NOU_TEST_IS_EQUAL(nou::AssertionSettings::printOnFail, true);
	NOU_TEST_IS_EQUAL(nou::AssertionSettings::callbackOnFail, nullptr);
	NOU_TEST_IS_EQUAL(nou::AssertionSettings::breakOnFail, false);
	NOU_TEST_IS_EQUAL(nou::AssertionSettings::exitOnFail, false);
	NOU_TEST_IS_EQUAL(nou::AssertionSettings::exitCode, -1);

	nou::AssertionSettings::callbackOnFail = assertCallbackSucceed; //test fails if this callback get called
	nou::AssertionSettings::exitOnFail = true; //-1 will be returned if exit() get called -> the test fails

	//succeeding assertion; test if the behaviors do not get triggered
	NOU_ASSERT(true);

	nou::AssertionSettings::callbackOnFail = assertCallbackFail;
	nou::AssertionSettings::exitCode = 0; //the assertion should terminate the test; it is not an error

	//fail assertion on purpose
	NOU_ASSERT(false);

	//Note: printOnFail and callbackOnFail are not testable

	//the test should be terminated by the previous NOU_ASSERT(), if the process reaches this line, the test 
	//has failed
	return -2; 
}