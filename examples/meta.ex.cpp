/**
File:          meta.ex.cpp
Author:        Lukas Reichmann
Creation Date: 27.09.2018 (dd.mm.yyyy)

An example on how to use the meta component.
*/

#ifndef NOU_TYPES_HPP
#    include "nostrautils/types.hpp"
#endif

#ifndef NOU_META_HPP
#    include "nostrautils/meta.hpp"
#endif

#include <iostream>
#include <typeinfo>

template<typename T>
void printType(const nou::char8 *msg)
{
    std::cout << msg << ": " << typeid(T).name() << "\n";
}

int main()
{
    printType<nou::IfType<true, nou::int32, nou::float32>>("nou::IfType<true, int, float>");
    printType<nou::IfType<false, nou::int32, nou::float32>>("nou::IfType<false, int, float>");

    printType<nou::AreSame<nou::int32, nou::float32>>("nou::AreSame<int, float>");
    printType<nou::AreSame<nou::int32, nou::int32>>("nou::AreSame<int, int>");

    printType<nou::AddConst<nou::int32>>("nou::AreSame<int, float>");
    printType<nou::RemoveConst<const nou::int32>>("nou::AreSame<int, int>");
}
