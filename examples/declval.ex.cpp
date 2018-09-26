
/**
File:          declval.ex.cpp
Author:        Lukas Reichmann
Creation Date: 26.09.2018 (dd.mm.yyyy)

An example on how to use the declval component.
*/

#ifndef NOU_TYPES_HPP
#    include "nostrautils/types.hpp"
#endif

#ifndef NOU_DECLVAL_HPP
#    include "nostrautils/declval.hpp"
#endif

#include <iostream>
#include <typeinfo>

template<typename T>
void print()
{
    // prints the type name that is the result of adding an \ilc{nou::int32} to an instance of \ilc{T} without
    // ever constructing an instance of the latter type.
    std::cout << typeid(decltype(nou::int32() + nou::declval<T>())).name() << "\n";
}

int main()
{
    print<nou::float32>();
}
