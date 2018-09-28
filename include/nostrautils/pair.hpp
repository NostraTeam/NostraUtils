#ifndef NOU_PAIR_HPP
#define NOU_PAIR_HPP

/**
\file nostrautils/pair.hpp

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0

\brief
A component that provides a data structure that can hold two values of different types.

\details
The main class in this component, \ilc{nou::Pair} behaves somewhat like \ilc{nou::Tuple} that
stores two values.

Accessing the values stored in a pair is done through the (public) member variables
\ilc{\link nou::Pair::first first\endlink} and \ilc{\link nou::Pair::second second\endlink}.
The class also supports structural bindings.

\par_example
\code{.cpp}
nou::Pair<nou::int32, nou::float32> pair = nou::pair(5, 10.0f);

nou::int32 integer = pair.first; //5
nou::int32 float = pair.second; //10.0f
\endcode

For a more detailed example, see \link pair.ex.cpp here\endlink.
*/

/**
\example pair.ex.cpp

An example that demonstrates the usage of the pair component.
*/

#ifndef NOU_META_HPP
#    include "nostrautils/meta.hpp"
#endif

#include <iostream>
#include <tuple>
#include <type_traits>

namespace nou
{
    /**
    \tparam A
    The type of the first member variable.

    \tparam B
    The type of the second member variable.

    \brief
    A simple class that can store two values of different types.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename A, typename B>
    class Pair final
    {
    public:
        using FirstType  = A;
        using SecondType = B;

        /**
        \brief
        The first value.

        \details
        This is equivalent to \ilc{first} and \ilc{left}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        A first;

        /**
        \brief
        The second value.

        \details
        This is equivalent to \ilc{second} and \ilc{right}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        B second;

        /**
        \tparam OA
        The type of the first value. The first value must be constructible from an instance of this type.

        \tparam OB
        The type of the second value. The second value must be constructible from an instance of this type.

        \param first
        The first value.

        \param second
        The second value.

        \brief
        Constructs a new instance from two l-value references.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        template<typename OA,
                 typename OB,
                 typename = nou::EnableIf<nou::IsConstructible<A, const OA &>::value>,
                 typename = nou::EnableIf<nou::IsConstructible<B, const OB &>::value>>
        Pair(const OA &first, const OB &second);

        /**
        \tparam OA
        The type of the first value. The first value must be constructible from an instance of this type.

        \tparam OB
        The type of the second value. The second value must be constructible from an instance of this type.

        \param first
        The first value.

        \param second
        The second value.

        \brief
        Constructs a new instance from two r-value references.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        template<typename OA,
                 typename OB,
                 typename = nou::EnableIf<nou::IsConstructible<A, OA &&>::value>,
                 typename = nou::EnableIf<nou::IsConstructible<B, OB &&>::value>>
        Pair(OA &&first, OB &&second);

        /**
        \param other
        The pair to copy from.

        \brief
        Constructs a new instance from another pair.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        Pair(const Pair<A, B> &other);

        /**
        \param other
        The pair to move from.

        \brief
        Constructs a new instance from another pair.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        Pair(Pair<A, B> &&other);

        /**
        \tparam OA
        The type of the first value of the pair to copy from. The first value must be constructible from an
        instance of this type.

        \tparam OB
        The type of the second value of the pair to copy from. The second value must be constructible from an
        instance of this type.

        \param other
        The pair to copy from.

        \brief
        Constructs a new instance from another pair.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        template<typename OA,
                 typename OB,
                 typename = nou::EnableIf<nou::IsConstructible<A, OA>::value>,
                 typename = nou::EnableIf<nou::IsConstructible<B, OB>::value>>
        Pair(const Pair<OA, OB> &other);

        /**
        \tparam OA
        The type of the first value of the pair to copy from. The first value must be constructible from an
        instance of this type.

        \tparam OB
        The type of the second value of the pair to copy from. The second value must be constructible from an
        instance of this type.

        \param other
        The pair to move from.

        \brief
        Constructs a new instance from another pair.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        template<typename OA,
                 typename OB,
                 typename = nou::EnableIf<nou::IsConstructible<A, OA &&>::value>,
                 typename = nou::EnableIf<nou::IsConstructible<B, OB &&>::value>>
        Pair(Pair<OA, OB> &&other);

        /**
        \param other
        The pair to copy from.

        \return
        A reference to the instance that the operator was called on.

        \brief
        Copies the values of the passed pair to the calling one.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        Pair &operator=(const Pair<A, B> &other);

        /**
        \param other
        The pair to move from.

        \return
        A reference to the instance that the operator was called on.

        \brief
        Moves the values of the passed pair to the calling one.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        Pair &operator=(Pair<A, B> &&other);

        /**
        \tparam OA
        The type of the first value of the pair to copy from. The first value must be constructible from an
        instance of this type.

        \tparam OB
        The type of the second value of the pair to copy from. The second value must be constructible from an
        instance of this type.

        \param other
        The pair to copy from.

        \return
        A reference to the instance that the operator was called on.

        \brief
        Copies the values of the passed pair to the calling one.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        template<typename OA,
                 typename OB,
                 typename = nou::EnableIf<nou::IsAssignable<A, OA>::value>,
                 typename = nou::EnableIf<nou::IsAssignable<B, OB>::value>>
        Pair &operator=(const Pair<OA, OB> &other);

        /**
        \tparam OA
        The type of the first value of the pair to copy from. The first value must be constructible from an
        instance of this type.

        \tparam OB
        The type of the second value of the pair to copy from. The second value must be constructible from an
        instance of this type.

        \param other
        The pair to move from.

        \return
        A reference to the instance that the operator was called on.

        \brief
        Moves the values of the passed pair to the calling one.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        template<typename OA,
                 typename OB,
                 typename = nou::EnableIf<nou::IsAssignable<A, OA &&>::value>,
                 typename = nou::EnableIf<nou::IsAssignable<B, OB &&>::value>>
        Pair &operator=(Pair<OA, OB> &&other);
    };

    /**
    \tparam A
    The type of the first member variable of the pair.

    \tparam B
    The type of the second member variable of the pair.

    \param first
    The first value.

    \param second
    The second value.

    \brief
    Constructs a new \ilc{nou::Pair} instance from two l-value references.

    \details
    This function can be used to write cleaner code, because it does not require the usage of explicit
    template parameters.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename A, typename B>
    Pair<A, B> pair(const A &first, const B &second);

    /**
    \param first
    The first value.

    \param second
    The second value.

    \brief
    Constructs a new \ilc{nou::Pair} instance from two r-value references.

    \details
    This function can be used to write cleaner code, because it does not require the usage of explicit
    template parameters.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename A, typename B>
    Pair<A, B> pair(A &&first, B &&second);

    template<typename A, typename B>
    template<typename OA, typename OB, typename, typename>
    Pair<A, B>::Pair(const OA &first, const OB &second) : first(first), second(second)
    {}

    template<typename A, typename B>
    template<typename OA, typename OB, typename, typename>
    Pair<A, B>::Pair(OA &&first, OB &&second) :
        first(static_cast<OA &&>(first)),
        second(static_cast<OB &&>(second))
    {}

    template<typename A, typename B>
    Pair<A, B>::Pair(const Pair<A, B> &other) : Pair(other.first, other.second)
    {}

    template<typename A, typename B>
    Pair<A, B>::Pair(Pair<A, B> &&other) :
        Pair(static_cast<A &&>(other.first), static_cast<B &&>(other.second))
    {}

    template<typename A, typename B>
    template<typename OA, typename OB, typename, typename>
    Pair<A, B>::Pair(const Pair<OA, OB> &other) : Pair(other.first, other.second)
    {}

    template<typename A, typename B>
    template<typename OA, typename OB, typename, typename>
    Pair<A, B>::Pair(Pair<OA, OB> &&other) :
        Pair(static_cast<OA &&>(other.first), static_cast<OB &&>(other.second))
    {}

    template<typename A, typename B>
    Pair<A, B> &Pair<A, B>::operator=(const Pair<A, B> &other)
    {
        first  = other.first;
        second = other.second;

        return *this;
    }

    template<typename A, typename B>
    Pair<A, B> &Pair<A, B>::operator=(Pair<A, B> &&other)
    {
        first  = static_cast<A &&>(other.first);
        second = static_cast<B &&>(other.second);

        return *this;
    }

    template<typename A, typename B>
    template<typename OA, typename OB, typename, typename>
    Pair<A, B> &Pair<A, B>::operator=(const Pair<OA, OB> &other)
    {
        first  = other.first;
        second = other.second;

        return *this;
    }

    template<typename A, typename B>
    template<typename OA, typename OB, typename, typename>
    Pair<A, B> &Pair<A, B>::operator=(Pair<OA, OB> &&other)
    {
        first  = static_cast<OA &&>(other.first);
        second = static_cast<OB &&>(other.second);

        return *this;
    }

    template<typename A, typename B>
    Pair<A, B> pair(const A &first, const B &second)
    {
        return Pair<A, B>(first, second);
    }

    template<typename A, typename B>
    Pair<A, B> pair(A &&first, B &&second)
    {
        return Pair<A, B>(static_cast<A &&>(first), static_cast<B &&>(second));
    }
} // namespace nou

#endif
