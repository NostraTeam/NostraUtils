
/**
File:          notnull.ex.cpp
Author:        Lukas Reichmann
Creation Date: 10.09.2018 (dd.mm.yyyy)

An example on how to use the pair component.
*/

#ifndef NOU_TYPES_HPP
#include "nostrautils/types.hpp"
#endif

#ifndef NOU_PAIR_HPP
#include "nostrautils/pair.hpp"
#endif
    
nou::Pair<nou::int32, nou::boolean> complicatedCalculation()
{
    //do a complicated calculation that may fail
    //...

    nou::int32 result; //the result of the calculation
    nou::boolean error; //true, if the calculation failed

    return nou::pair(result, error); //return both the result and the error state
}

int main()
{
    //a pair stores two values of (possibly) different types
    nou::Pair<nou::int32, nou::float32> pair = nou::pair(5, 3.0f);

    //access using .first and .second
    pair.first = 5;
    pair.second = 6.0f;

    //.a and .b are aliases for .first and .second; there are additional aliases called .left and .right
    pair.a = 10;
    pair.b = 15.0f;

    //pairs provide a nice way to return two values
    //result now holds the result of the calculation and error the error state
    auto[result, error] = complicatedCalculation();

    if (!error)
    {
        //do something with the result
        //...
    }
}