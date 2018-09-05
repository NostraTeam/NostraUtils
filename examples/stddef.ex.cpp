
/**
File:          stddef.ex.cpp
Author:        Lukas Reichmann
Creation Date: 04.09.2018 (dd.mm.yyyy)

An example on how to use the macros defined in the stddef component.
*/

#ifndef NOU_STDDEF_HPP
#include "nostrautils/stddef.hpp"
#endif

#include <iostream>

/*
If the current operating system is Windows, the string "Windows" will be printed to the standard output 
stream. If the operating system is Linux, the string "Linux" will be printed to the standard output stream. 
If another operating system is being used, "Other operating system" will be printed.
 */
void printOperatingSystem()
{
#if NOU_OS == NOU_OS_WINDOWS

	std::cout << "Windows\n";

#elif NOU_OS == NOU_OS_LINUX

	std::cout << "Linux\n";

#else

	std::cout << "Other operating system\n";

#endif
}

/*
If the current compiler is MSVC, the string "MSVC" will be printed to the standard output stream. If the 
compiler is GCC, the string "GCC" will be printed to the standard output stream. If another compiler is being
used, "Other compiler" will be printed.
 */
void printCompiler()
{
#if NOU_COMPILER == NOU_COMPILER_MSVC

	std::cout << "MSVC\n";

#elif NOU_COMPILER == NOU_COMPILER_GCC

	std::cout << "GCC\n";

#else

	std::cout << "Other compiler\n";

#endif
}

int main()
{
	printOperatingSystem();
	printCompiler();

	std::cout << NOU_STRINGIFY(sometext) << "\n"; //NOU_STRINGIFY(sometext) expands to "sometext"
	std::cout << NOU_LINE_STRING << "\n"; //NOU_LINE_STRING expands to "86"

	//expansion is platform dependent; but it should expand to something that contains the name of the 
	//function (main)
	std::cout << NOU_FUNC_NAME << "\n"; 
}