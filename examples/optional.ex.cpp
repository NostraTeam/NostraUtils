/**
File:          optional.ex.cpp
Author:        Lukas Reichmann
Creation Date: 13.10.2018 (dd.mm.yyyy)

An example on how to use the optional component.
*/

#ifndef NOU_TYPES_HPP
#    include "nostrautils/types.hpp"
#endif

#ifndef NOU_OPTIONAL_HPP
#    include "nostrautils/optional.hpp"
#endif

#include <iostream>

class Test
{
public:
    Test()
    {
        std::cout << "Test::Test()\n";
    }
};

class Array
{
private:
    nou::int32 m_data[5];

public:
    nou::Optional<nou::int32 *> operator[](nou::sizeType index)
    {
        if(index < 5) // if the index is in the range of the array, return the value
            return m_data + index;
        else // if not, return an invalid instance of nou::Optional
            return nou::invalidOpt();
    }
};

int main()
{ 
     // use optional to initialize a variable at a later point
     {
         nou::Optional<Test> opt; // the wrapped object is not initialized yet

         std::cout << "opt.isValid(): " << opt.isValid() << "\n";

         opt = Test(); // the wrapped object is now initialized

         std::cout << "opt.isValid(): " << opt.isValid() << "\n";
     }

    std::cout << "======\n";

     // use optional to return (potentially) invalid values
     {
         Array array;

         nou::Optional<nou::int32 *> opt0 = array[0];
         std::cout << "opt0.isValid(): " << opt0.isValid() << "\n";

         nou::Optional<nou::int32 *> opt1 = array[1];
         std::cout << "opt1.isValid(): " << opt1.isValid() << "\n";

         nou::Optional<nou::int32 *> opt2 = array[2];
         std::cout << "opt2.isValid(): " << opt2.isValid() << "\n";

         nou::Optional<nou::int32 *> opt3 = array[3];
         std::cout << "opt3.isValid(): " << opt3.isValid() << "\n";

         nou::Optional<nou::int32 *> opt4 = array[4];
         std::cout << "opt4.isValid(): " << opt4.isValid() << "\n";

         nou::Optional<nou::int32 *> opt5 = array[5];
         std::cout << "opt5.isValid(): " << opt5.isValid() << "\n";
     }
}
