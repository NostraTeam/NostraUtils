#ifndef NOU_DECLVAL_HPP
#define NOU_DECLVAL_HPP

/**
\file nostrautils/assert.hpp

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0

\brief
A component that provides the function \ilc{nou::declval()}.

\details
\ilc{nou::declval()} is a function that may only be used in unevaluated context (such as in the parameter list
of \ilc{declval(}). It will then return an r-value reference to an instance of the type that was passed to it,
or \ilc{void} if the passed type is \ilc{void}.

The function is useful if an instance of a type is needed, but it is not known what constructor(s) that type
has.

\par_example
\code{.cpp}
#include <iostream>
#include <typeinfo>

template<typename T>
void print()
{
    // prints the type name that is the result of adding an \ilc{nou::int32} to an instance of \ilc{T} without
    // ever constructing an instance of the latter type.
    std::cout << typeid(decltype(nou::int32() + nou::declval<T>())).name() << "\n";
}
\endcode

Note that, if the function is called outside unevaluated context, the call will most likely result in an
linker error.

The same example can also be found \link declval.ex.cpp here\endlink.
*/

/**
\example declval.ex.cpp

An example that demonstrates the usage of the declval component.
*/

#include <type_traits>

namespace nou
{
    /**
     * \tparam T
     * The type to obtains an instance of.
     *
     * \brief
     * Returns a r-value reference to an instance of the passed type \p T.
     *
     * \warning
     * May only be used in unevaluated context.
     *
     * \return A r-value reference to an instance of the passed type \p T.
     */
    template<typename T>
    std::add_rvalue_reference_t<T> declval();

    ///\cond
    template<>
    void declval<void>();
    ///\endcond
} // namespace nou

#endif
