
/**
File:          notnull.ex.cpp
Author:        Lukas Reichmann
Creation Date: 08.09.2018 (dd.mm.yyyy)

An example on how to use the notnull component.
*/

#ifndef NOU_TYPES_HPP
#    include "nostrautils/types.hpp"
#endif

#ifndef NOU_NOTNULL_HPP
#    include "nostrautils/notnull.hpp"
#endif

void someFunction(nou::NotNull<nou::int32 *> ptr)
{
    // some logic that requires ptr to be not nullptr
}

int main()
{
    nou::int32 i     = 5;
    nou::int32 *iPtr = &i;

    // construct a new instance
    nou::NotNull<nou::int32 *> notNull = iPtr;

    // would result in a compile error
    // nou::NotNull<nou::int32*> notNull1 = nullptr;

    // would result in a runtime error (abort() will be called)
    // nou::int32 *null = nullptr;
    // nou::NotNull<nou::int32*> notNull2 = null;

    // nou::NotNull supports the common pointer operators, including arithmetic and comparison operators
    nou::int32 iCpy = *notNull;

    // call a function that takes a nou::NotNull as its parameter
    someFunction(&i);
}