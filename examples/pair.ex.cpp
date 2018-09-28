/**
File:          pair.ex.cpp
Author:        Lukas Reichmann
Creation Date: 28.09.2018 (dd.mm.yyyy)

An example on how to use the pair component.
*/

#ifndef NOU_TYPES_HPP
#    include "nostrautils/types.hpp"
#endif

#ifndef NOU_PAIR_HPP
#    include "nostrautils/pair.hpp"
#endif

#include <iostream>

nou::Pair<nou::int32, nou::float32> func()
{
    return nou::pair(20, 0.5f);
}

int main()
{
    // create a new pair, with the types int and float.
    // using auto and nou::pair() avoids having to use explicit template parameters
    auto pair = nou::pair(5, 1.0f);

    // access the two components
    std::cout << pair.first << " " << pair.second << "\n";

    // pairs support structural bindings
    auto [a, b] = func();

    std::cout << a << " " << b << "\n";
}
