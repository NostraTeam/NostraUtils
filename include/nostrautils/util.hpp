#ifndef NOU_DECLVAL_HPP
#define NOU_DECLVAL_HPP

/**
\file nostrautils/util.hpp

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0

\brief
A component that provides various utility functions, such as \ilc{nou::declval()} or \ilc{nou::move()}.

\details
Since the functions in this component are very different, there is no description and examples here. Instead,
they are in the documentation of each of the functions.

A collection of all of the examples in the component can also be found \link util.ex.cpp here\endlink.
*/

/**
\example util.ex.cpp

An example that demonstrates the functions in the util component.
*/

#ifndef NOU_META_HPP
#    include "nostrautils/meta.hpp"
#endif

namespace nou
{
    /**
    \tparam T
    The type to obtains an instance of.

    \return A r-value reference to an instance of the passed type \ilc{T}. If \ilc{T} is \ilc{void},
    \ilc{void} will be returned.

    \brief
    Returns a r-value reference to an instance of the passed type \ilc{T}.

    \details
    A function that, in unevaluated context, returns an instance of \ilc{T}.

    The function is useful if an instance of a type is needed, but it is not known what constructor(s) that
    type has.

    \par_example
    \code{.cpp}
    #include <iostream>
    #include <typeinfo>

    template<typename T>
    void print()
    {
        // prints the type name that is the result of adding an \ilc{nou::int32} to an instance of \ilc{T}
        // without  ever constructing an instance of the latter type.
        std::cout << typeid(decltype(nou::int32() + nou::declval<T>())).name() << "\n";
    }
    \endcode

    \warning
    May only be used in unevaluated context or a call will most likely result in a link-time error.
    */
    template<typename T>
    nou::AddRValueReferenceType<T> declval() noexcept;

    ///\cond
    template<>
    void declval<void>() noexcept;
    ///\endcond

    /**
    \tparam T
    The type of the object to move.

    \param t
    The object to move.

    \return
    \ilc{t} as r-value reference.

    \brief
    Converts the passed object to an r-value reference, allowing it to be moved using move- construction or
    assignment (if the type \ilc{T} supports it).

    \details
    Moving an object allows for an efficient transfer of its data. \ilc{nou::move()} explicitly requests that
    an object should be move.

    \par_example
    \code{.cpp}
    #include <iostream>

    class Test
    {
    public:
        Test() = default;

        Test(const Test &other)
        {
            std::cout << "Test::Copy()\n";
        }

        Test(Test &&other)
        {
            std::cout << "Test::Move()\n";
        }
    };

    void moveTest()
    {
        Test t1;

        Test t2 = t1; // print: Test::Copy()
        Test t3 = nou::move(t1); //print: Test::Move()
    }
    \endcode
    */
    template<typename T>
    inline RemoveReferenceType<T> move(T &&t)
    {
        return static_cast<RemoveReferenceType<T> &&>(t);
    }

    /**
    \tparam T
    The type of the parameter. This type can never be deduced and always needs to be explicitly passed.

    \param t
    The object to forward.

    \return
    \ilc{t} as l-value reference if it is an l-value reference or \ilc{t} as r-value reference if it is an
    r-value reference.

    \brief
    Depending on \ilc{T}, the passed object will be either forwarded as r-value- or l-value reference.

    \details
    \par_example
    \code{.cpp}
    #include <iostream>

    class Test
    {
    public:
        nou::boolean m_wasCopied;
        nou::boolean m_wasMoved;

        Test() : m_wasCopied(false), m_wasMoved(false){};

        Test(const Test &other) : m_wasCopied(true), m_wasMoved(false)
        {}

        Test(Test &&other) : m_wasCopied(false), m_wasMoved(true)
        {}
    };

    template<typename T>
    void forwardTest(T &&t)
    {
        Test test = nou::forward<Test>(t);

        std::cout << "Copied: " << test.m_wasCopied << "; Moved: " << test.m_wasMoved << "\n";
    }

    int main()
    {
        Test test;

        forwardTest(test);   // passing l-value reference
        forwardTest(Test()); // passing r-value reference
    }
    \endcode
    */
    template<typename T>
    constexpr T &&forward(RemoveReferenceType<T> &t)
    {
        return static_cast<T &&>(t);
    }

    ///\cond
    template<typename T>
    constexpr T &&forward(RemoveReferenceType<T> &&t)
    {
        return static_cast<T &&>(t);
    }
    ///\endcond

} // namespace nou

#endif
