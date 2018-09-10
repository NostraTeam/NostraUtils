#ifndef NOU_ASSERT_HPP
#    include "nostrautils/assert.hpp"
#endif

#include <cstdlib>
#include <iostream>

namespace nou
{
    boolean AssertionSettings::breakOnFail = false;

    boolean AssertionSettings::printOnFail = true;

    boolean AssertionSettings::exitOnFail = false;

    int AssertionSettings::exitCode = -1;

    AssertionSettings::CallbackType AssertionSettings::callbackOnFail = nullptr;

    namespace internal
    {
        void debugbreak()
        {
#if NOU_COMPILER == NOU_COMPILER_MSVC
            __debugbreak();
#endif
        }

        void assertImpl(boolean b, const char8 *msg, const char8 *fnName, uint32 line, const char8 *file)
        {
            if(!b)
            {
                if(AssertionSettings::printOnFail)
                    std::cout << msg << "\n";

                if(AssertionSettings::callbackOnFail)
                    AssertionSettings::callbackOnFail(msg, fnName, line, file);

                if(AssertionSettings::breakOnFail)
                    debugbreak();

                if(AssertionSettings::exitOnFail)
                    std::exit(AssertionSettings::exitCode);
            }
        }
    } // namespace internal
} // namespace nou