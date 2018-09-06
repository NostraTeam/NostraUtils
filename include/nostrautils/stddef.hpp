#ifndef NOU_STDDEF_HPP
#define NOU_STDDEF_HPP

/**
\file nostrautils/stddef.hpp

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0

\brief
A component that provides preprocessor macros that are usually used for platform detection and to abstract
code from a single platform.

\details
For a user of the library, this component mainly provides macros to detect the platform that the program is
currently being built on at pre-processing time.

In addition to that, it provides basic utility macros and shared library related macros that are only used 
within the library.

The basic system for platform detection uses detectable attributes (such as a compiler) and values (also 
called attribute identifiers) that those attributes may have. In other words, an attribute will always be 
compared to a value that such an attribute might have.

The attributes are:
<ul>
	<li>compiler</li>
	<li>operating system</li>
</ul>

<b>Note:</b> This system does NOT use defined or undefined macros - all of the macros are always defined, but
the value that the attributes expand to changes.

<b>Example:</b>
\code{.cpp}
#if NOU_OS == NOU_OS_LINUX
	//This will only get compiled when compiling under Linux
#elif NOU_OS == NOU_OS_WINDOWS
	//This will only get compiled when compiling under Windows
#endif
\endcode

For a more detailed example, see \link stddef.ex.cpp here\endlink.
*/

/**
\example stddef.ex.cpp

An example that demonstrates the usage of the macros provided by the stddef component.
*/

/**
\brief 
The operating system attribute identifier for Microsoft Windows.

\details
This macro is always defined, to check if the current operating system is Windows, use
\code{.cpp}
#if NOU_OS == NOU_OS_WINDOWS
\endcode

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_OS_WINDOWS
#define NOU_OS_WINDOWS 0
#endif

/**
\brief 
The operating system attribute identifier for Linux.

\details
This macro is always defined, to check if the current operating system is Linux, use
\code{.cpp}
#if NOU_OS == NOU_OS_LINUX
\endcode

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_OS_LINUX
#define NOU_OS_LINUX 1
#endif

/**
\brief 
The operating system attribute identifier for Cygwin.

\details
This macro is always defined, to check if the program is being compiled on the Cygwin platform, use
\code{.cpp}
#if NOU_OS == NOU_OS_CYGWIN
\endcode

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_OS_CYGWIN
#define NOU_OS_CYGWIN 2
#endif

/**
\brief
The operating system attribute identifier for MinGW.

\details
This macro is always defined, to check if the program is being compiled on the MinGW platform, use
\code{.cpp}
#if NOU_OS == NOU_OS_MINGW
\endcode

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_OS_MINGW
#define NOU_OS_MINGW 2
#endif

/**
\brief 
The operating system attribute identifier for Mac OSX.

\details
This macro is always defined, to check if the current operating system is Mac OSX, use
\code{.cpp}
#if NOU_OS == NOU_OS_MAC
\endcode

\note
As of now, NostraUtils does not support Mac OSX and this macro is only defined for future use.

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_OS_MAC
#define NOU_OS_MAC 4
#endif

/**
\brief 
The operating system attribute identifier for an unknown system.

\details
This macro is always defined, to check if the current operating system is unknown, use
\code{.cpp}
#if NOU_OS == NOU_OS_UNKNOWN
\endcode
An unknown operating system does not cause an error per-se, however, it may cause major problems because
the library does not properly implement functionality for an unknown operating system and often an 
<code>#error</code> directive will be triggered.

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_OS_UNKNOWN
#define NOU_OS_UNKNOWN 5
#endif

/**
\brief 
The operating system attribute identifier for the Doxygen documentation.

\details
This macro is always defined, but it does not serve any purpose when using the library. However, while
doxygen generated the documentation, NOU_OS will be set to this attribute identifier.

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_OS_DOXYGEN    
#define NOU_OS_DOXYGEN 6
#endif

/**
\brief 
Defined as the attribute identifier of the current operating system.

\details
To check for a certain operating system, use
\code{.cpp}
#if NOU_OS == NOU_OS_*
\endcode
and replace * with the operating system name.
If the current operating system is unknown, the value will be NOU_OS_UNKNOWN.
An unknown operating system does not cause an error per-se, however, it may cause major problems (e.g. the
value for NOU_OS_LIBRARY will not be set properly).

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_OS
#    if defined __linux__
#    define NOU_OS NOU_OS_LINUX

#    elif defined __CYGWIN__
#    define NOU_OS NOU_OS_CYGWIN

#    elif defined __MINGW32__
#    define NOU_OS NOU_OS_MINGW

#    elif defined _WIN32 //must be after MinGW and Cygwin
#    define NOU_OS NOU_OS_WINDOWS

//#    elif defined __APPLE__
//#    define NOU_OS NOU_OS_MAC

#    elif defined NOU_DOXYGEN //NOU_DOXYGEN is defined in the Doxyfile
#    define NOU_OS NOU_OS_DOXYGEN

#    else
#    define NOU_OS NOU_OS_UNKNOWN

#    endif
#endif

/**
\brief 
The compiler attribute identifier for MSVC.

\details
This macro is always defined, to check if the current compiler is MVSC, use
\code{.cpp}
#if NOU_COMPILER == NOU_COMPILER_VISUAL_CPP
\endcode

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_COMPILER_MSVC
#define NOU_COMPILER_MSVC 0
#endif

/**
\brief 
The compiler attribute identifier for GNU GCC or g++.

\details
This macro is always defined, to check if the current compiler is GNU GCC or g++, use
\code{.cpp}
#if NOU_COMPILER == NOU_COMPILER_GCC
\endcode

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_COMPILER_GCC
#define NOU_COMPILER_GCC     1
#endif						    

/**
\brief 
The compiler attribute identifier for Clang.

\details
This macro is always defined, to check if the current compiler is Clang, use
\code{.cpp}
#if NOU_COMPILER == NOU_COMPILER_CLANG
\endcode

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_COMPILER_CLANG	    
#define NOU_COMPILER_CLANG   2
#endif						    

/**
\brief 
The compiler attribute identifier for an unknown compiler.

\details
This macro is always defined, to check if the current compiler is unknown, use
\code{.cpp}
#if NOU_COMPILER == NOU_COMPILER_UNKNOWN
\endcode
An unknown compiler does not cause an error per-se, however, it may cause major problems (e.g. the
value of NOU_FUNC will not be set properly). In addition to that, if the operating system is unknown, the 
library compiles on a system that is not officially supported. In addition to that, if the compiler is 
unknown, that compiler is not officially supported.

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_COMPILER_UNKNOWN    
#define NOU_COMPILER_UNKNOWN 3
#endif

/**
\brief 
The compiler attribute identifier for the Doxygen documentation.

\details
The compiler identifier for an Doxygen documentation. This macro is always defined, but it does not serve any
purpose when using the library. However, while doxygen generated the documentation, NOU_COMPILER will be set 
to this attribute identifier.

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_COMPILER_DOXYGEN    
#define NOU_COMPILER_DOXYGEN 4
#endif

/**
\brief 
Defined as the attribute identifier of the current compiler.

\details
To check for a certain compiler, use
\code{.cpp}
#if NOU_COMPILER == NOU_COMPILER_*
\endcode
and replace * with the compiler name.

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_COMPILER

#    ifdef __clang__ //This has to be placed first (b/c Clang also defines __GNUC__ and _MSC_VER)
#    define NOU_COMPILER NOU_COMPILER_CLANG
#    elif defined _MSC_VER
#    define NOU_COMPILER NOU_COMPILER_MSVC
#    elif defined __GNUC__ 
#    define NOU_COMPILER NOU_COMPILER_GCC
#    elif defined NOU_DOXYGEN
#    define NOU_COMPILER NOU_COMPILER_DOXYGEN
#    else 
#    define NOU_COMPILER NOU_COMPILER_UNKNOWN
#    endif

#endif

/**
\brief 
A macro that is used to export functions into shared libraries.

\details
Prefixing a function (or any exportable symbol) with this macro will cause it to be exported into a shared 
library if one is being build. If a shared library is not being built, but this macro is still in use, it
may cause problems.

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_EXPORT_FUNC

#	if NOU_COMPILER == NOU_COMPILER_MSVC
#		define NOU_EXPORT_FUNC __declspec(dllexport)
#	else
#	    define NOU_EXPORT_FUNC
#	endif

#endif

/**
\brief 
A macro that is used to import functions a function from a library.

\details
Prefixing a function (or any exportable symbol) with this macro will cause it to be imported from a shared
library if a library that is linked against contains the symbol. If no such symbol exists, but this macro is 
still in use, it may cause problems.

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_IMPORT_FUNC

#	if NOU_COMPILER == NOU_COMPILER_MSVC
#		define NOU_IMPORT_FUNC __declspec(dllimport)
#	else
#	    define NOU_IMPORT_FUNC
#	endif

#endif

/**
\brief
A macro that either exports or imports a function from a library (if desired).

\details
This macro will import, if NOU_EXPORT_SYMBOLS is defined and export if it is not. In addition to that, it 
will behave correctly depending on whether a shared or static library is being build by using 
NOU_EXPORT_SYMBOLS. Whether or not NOU_DLL and NOU_EXPORT_SYMBOLS are defined is handled by CMake and does 
not involve any user involvement.

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_FUNC
#    ifdef NOU_BUILD_DLL
#        ifdef NOU_EXPORT_SYMBOLS
#            define NOU_FUNC NOU_IMPORT_FUNC
#        else
#            define NOU_FUNC NOU_EXPORT_FUNC
#        endif
#    else
#        define NOU_FUNC
#    endif
#endif

/**
\param ... 
The expression to convert.

\brief 
Converts any expression into a const char[].

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_STRINGIFY
#define NOU_STRINGIFY(...) #__VA_ARGS__
#endif 

/**
\brief 
Returns the result of __LINE__ as a C-string.

\details
This additional macro is required, because simply using NOU_STRINGIFY(__LINE__) would not work.

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_LINE_STRING
///\cond
#define NOU_INTERNAL_LINE_STRING_HELPER(str) NOU_STRINGIFY(str)
///\endcond
#define NOU_LINE_STRING NOU_INTERNAL_LINE_STRING_HELPER(__LINE__)
#endif

/**
\brief 
Expands to a string literal with the function signature of the function that this macro was placed in.

\note
Although this macro is platform independent, it is not guaranteed that using this macro in the same function
on different platforms also produces the same result.

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0
*/
#ifndef NOU_FUNC_NAME

#	if NOU_COMPILER == NOU_COMPILER_MSVC
#	define NOU_FUNC_NAME __FUNCSIG__
#	elif NOU_COMPILER == NOU_COMPILER_GCC
#	define NOU_FUNC_NAME NOU_STRINGIFY(__PRETTY_FUNCTION__)
#	elif NOU_COMPILER == NOU_COMPILER_CLANG
#	define NOU_FUNC_NAME NOU_STRINGIFY(__PRETTY_FUNCTION__)
#	elif NOU_COMPILER == NOU_COMPILER_DOXYGEN
#	define NOU_FUNC_NAME <compiler dependent> //no proper value required
#	else
#	define NOU_FUNC_NAME __func__
#	endif

#endif



#endif
