
/**
File:          assert.ex.cpp
Author:        Lukas Reichmann
Creation Date: 05.09.2018 (dd.mm.yyyy)

An example on how to use the assert component.
*/

#ifndef NOU_TYPES_HPP
#    include "nostrautils/types.hpp"
#endif

#ifndef NOU_ASSERT_HPP
#    include "nostrautils/assert.hpp"
#endif

#include <iostream>

void assertCallback(const nou::char8 *msg, const nou::char8 *fnName, nou::uint32 line, const nou::char8 *file)
{
    // gets executed when an assertion fails
    std::cout << "callback!" << std::endl;
}

// a simple function that will check if i is larger or equal to ten
void someFunction(nou::int32 i)
{
    NOU_ASSERT(i >= 10);

    // logic that relies on i being larger or equal to ten goes here
}

int main()
{
    // prints a message to std::cout on failure; by default this is always enabled
    nou::AssertionSettings::printOnFail = true;

    // calls printOnFail() on failure; by default, no callback is set and the behavior is disabled
    nou::AssertionSettings::callbackOnFail = assertCallback;

    // if possible, triggers a debug breakpoint; by default this is always disabled
    nou::AssertionSettings::breakOnFail = true;

    // exits the program using exit(); by default this is always disabled
    nou::AssertionSettings::exitOnFail = true;

    // set the exit code that is used when the behavior exitOnFail is triggered; by default this is -1
    nou::AssertionSettings::exitCode = 5;

    // exits the program using abort(); by default this is always disabled
    // Note that during this example, abort() will never be called since exit() will be called first
    nou::AssertionSettings::abortOnFail = true;

    someFunction(5);  // assertion fails
    someFunction(10); // assertion succeeds
}