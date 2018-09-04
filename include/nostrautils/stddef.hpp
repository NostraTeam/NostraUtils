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
compared to a value that that attribute might have.

The attributes are:
<ul>
	<li>compiler</li>
	<li>operating system</li>
	<li>operating system library (POSIX or Windows.h)</li>
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
*/
#ifndef NOU_OS_LINUX
#define NOU_OS_LINUX 1
#endif

/**
\brief 
The operating system attribute identifier for Unix.

\details
This macro is always defined, to check if the current operating system is Unix, use
\code{.cpp}
#if NOU_OS == NOU_OS_UNIX
\endcode
*/
#ifndef NOU_OS_UNIX
#define NOU_OS_UNIX 2
#endif

/**
\brief 
The operating system attribute identifier for Macintosh.

\details
This macro is always defined, to check if the current operating system is Macintosh, use
\code{.cpp}
#if NOU_OS == NOU_OS_MAC
\endcode
*/
#ifndef NOU_OS_MAC
#define NOU_OS_MAC 2
#endif

/**
\brief 
The operating system attribute identifier for an unknown system.

\details
This macro is always defined, to check if the current operating system is unknown, use
\code{.cpp}
#if NOU_OS == NOU_OS_UNKNOWN
\endcode
An unknown operating system does not cause an error per-se, however, it may cause major problems (e.g. the
value for NOU_OS_LIBRARY will not be set properly).
*/
#ifndef NOU_OS_UNKNOWN
#define NOU_OS_UNKNOWN 3
#endif

/**
\brief 
The operating system attribute identifier for the Doxygen documentation.

\details
This macro is always defined, but it does not serve any purpose when using the library. However, while
doxygen generated the documentation, NOU_OS will be set to this attribute identifier.
*/
#ifndef NOU_OS_DOXYGEN    
#define NOU_OS_DOXYGEN 4
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
*/
#ifndef NOU_OS
#    ifdef _WIN32
#    define NOU_OS NOU_OS_WINDOWS

#    elif defined __linux__
#    define NOU_OS NOU_OS_LINUX

#    elif defined __unix__
#    define NOU_OS NOU_OS_UNIX

#    elif defined __APPLE__
#    define NOU_OS NOU_OS_MAC

#    elif defined NOU_DOXYGEN //NOU_DOXYGEN is defined in the Doxyfile
#    define NOU_OS NOU_OS_DOXYGEN

#    else
#    define NOU_OS NOU_OS_UNKNOWN

#    endif
#endif

/**
\brief 
The operating system library attribute identifier for the Windows.h library.

\details
This macro is always defined, to check if the current operating system supports the Windows.h library, use
\code{.cpp}
#if NOU_OS_LIBRARY == NOU_OS_LIBRARY_WIN_H
\endcode
*/
#ifndef NOU_OS_LIBRARY_WIN_H
#define NOU_OS_LIBRARY_WIN_H 0
#endif

/**
\brief 
The operating system library attribute identifier for the POSIX library.

\details
This macro is always defined, to check if the current operating system supports the POSIX library, use
\code{.cpp}
#if NOU_OS_LIBRARY == NOU_OS_LIBRARY_POSIX
\endcode
*/
#ifndef NOU_OS_LIBRARY_POSIX
#define NOU_OS_LIBRARY_POSIX 1
#endif

/**
\brief 
The operating system library attribute identifier for the Doxygen documentation.

\details
This macro is always defined, but it does not serve any purpose when using the library. However, while
doxygen generated the documentation, NOU_OS_LIBRARY will be set to this attribute identifier.
*/
#ifndef NOU_OS_LIBRARY_DOXYGEN    
#define NOU_OS_LIBRARY_DOXYGEN 2
#endif

/**
\brief 
Defined as the attribute identifier of the currently used operating system library.

\details
To check for a certain library, use
\code{.cpp}
#if NOU_OS_LIBRARY == NOU_OS_LIBRARY_*
\endcode
and replace * with the library name.
If there is no appropriate library available, a \#error directive will be triggered.
*/
#ifndef NOU_OS_LIBRARY
#	if NOU_OS == NOU_OS_WINDOWS
#	define NOU_OS_LIBRARY NOU_OS_LIBRARY_WIN_H

#	elif NOU_OS == NOU_OS_LINUX || NOU_OS == NOU_OS_UNIX || NOU_OS == NOU_OS_MAC
#	define NOU_OS_LIBRARY NOU_OS_LIBRARY_POSIX

#   elif NOU_OS == NOU_OS_DOXYGEN
#   define NOU_OS_LIBRARY NOU_OS_LIBRARY_DOXYGEN

#	else
#	error Both Windows.h and POSIX are not supported.

#	endif
#endif

#endif

/**
\brief 
The compiler attribute identifier for MSVC.

\details
This macro is always defined, to check if the current compiler is MVSC, use
\code{.cpp}
#if NOU_COMPILER == NOU_COMPILER_VISUAL_CPP
\endcode
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
*/
#ifndef NOU_COMPILER_GCC
#define NOU_COMPILER_GCC        1
#endif						    

/**
\brief 
The compiler attribute identifier for Clang.

\details
This macro is always defined, to check if the current compiler is Clang, use
\code{.cpp}
#if NOU_COMPILER == NOU_COMPILER_CLANG
\endcode
*/
#ifndef NOU_COMPILER_CLANG	    
#define NOU_COMPILER_CLANG      2
#endif						    

/**
\brief 
The compiler attribute identifier for Intel C++.

\details
This macro is always defined, to check if the current compiler is Intel C++, use
\code{.cpp}
#if NOU_COMPILER == NOU_COMPILER_INTEL_CPP
\endcode
*/
#ifndef NOU_COMPILER_INTEL_CPP  
#define NOU_COMPILER_INTEL_CPP  3
#endif						    

/**
\brief 
The compiler attribute identifier for MinGW.

\details
This macro is always defined, to check if the current compiler is MinGW,
use
\code{.cpp}
#if NOU_COMPILER == NOU_COMPILER_MIN_GW
\endcode
*/
#ifndef NOU_COMPILER_MIN_GW	    
#define NOU_COMPILER_MIN_GW     4
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
value of NOU_FUNC will not be set properly).
*/
#ifndef NOU_COMPILER_UNKNOWN    
#define NOU_COMPILER_UNKNOWN    5
#endif

/**
\brief 
The compiler attribute identifier for the Doxygen documentation.

\details
The compiler identifier for an Doxygen documentation. This macro is always defined, but it does not serve any
purpose when using the library. However, while doxygen generated the documentation, NOU_COMPILER will be set 
to this attribute identifier.
*/
#ifndef NOU_COMPILER_DOXYGEN    
#define NOU_COMPILER_DOXYGEN    6
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
*/
#ifndef NOU_COMPILER

#    ifdef __clang__ //This has to be placed first (b/c Clang also defines __GNUC__ and _MSC_VER)
#    define NOU_COMPILER NOU_COMPILER_CLANG
#    elif defined _MSC_VER
#    define NOU_COMPILER NOU_COMPILER_MSVC
#    elif defined __GNUC__ 
#    define NOU_COMPILER NOU_COMPILER_GCC
#    elif defined __INTEL_COMPILER
#    define NOU_COMPILER NOU_COMPILER_INTEL_CPP
#    elif defined __MINGW32__
#    define NOU_COMPILER NOU_COMPILER_MIN_GW
#    elif defined __DOXYGEN__
#    define NOU_COMPILER NOU_COMPILER_DOXYGEN
#    else 
#    define NOU_COMPILER NOU_COMPILER_UNKNOWN
#    endif

/**
\brief 
A macro that is used to export functions into shared libraries.

\details
Prefixing a function (or any exportable symbol) with this macro will cause it to be exported into a shared 
library if one is being build. If a shared library is not being built, but this macro is still in use, it
may cause problems.
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

#endif

/**
\param ... 
The expression to convert.

\brief 
Converts any expression into a const char[].
*/
#ifndef NOU_STRINGIFY
#define NOU_STRINGIFY(...) #__VA_ARGS__
#endif 

/**
\brief 
Returns the result of __LINE__ as a C-string.

\details
This additional macro is required, because simply using NOU_STRINGIFY(__LINE__) would not work.
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
*/
#ifndef NOU_FUNC_NAME

#	if NOU_COMPILER == NOU_COMPILER_MSVC
#	define NOU_FUNC_NAME __FUNCSIG__
#	elif NOU_COMPILER == NOU_COMPILER_GCC
#	define NOU_FUNC_NAME NOU_STRINGIFY(__PRETTY_FUNCTION__)
#	elif NOU_COMPILER == NOU_COMPILER_CLANG
#	define NOU_FUNC_NAME NOU_STRINGIFY(__PRETTY_FUNCTION__)
#	elif NOU_COMPILER == NOU_COMPILER_INTEL_CPP
#	define NOU_FUNC_NAME __func__ ///\Todo check
#	elif NOU_COMPILER == NOU_COMPILER_MIN_GW
#	define NOU_FUNC_NAME __func__ ///\Todo check
#	elif NOU_COMPILER == NOU_COMPILER_DOXYGEN
#	define NOU_FUNC_NAME __FUNCSIG__
#	else
#	define NOU_FUNC_NAME __func__
#	endif

#endif