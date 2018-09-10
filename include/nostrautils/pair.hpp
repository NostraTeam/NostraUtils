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
\ilc{\link nou::Pair::first first\endlink} and \ilc{\link nou::Pair::second second\endlink}. In additions to
these two, there are two alias names for each of the variables:
<table>
    <tr>
        <th>Actual Name</th>
        <th>Alias Names</th>
    </tr>
    <tr>
        <td rowspan="2">first</td>
        <td>a</td>
    </tr>
    <tr>
        <td>left</td>
    </tr>
    <tr>
        <td rowspan="2">second</td>
        <td>b</td>
    </tr>
    <tr>
        <td>right</td>
    </tr>
</table>
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

#ifndef NOU_TYPES_HPP
#include <nostrautils/types.hpp>
#endif

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
        using FirstType = A;
        using SecondType = B;

        /**
        \brief
        The first value.

        \details
        This is equivalent to \ilc{a} and \ilc{left}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        A first;

        /**
        \brief
        The second value.

        \details
        This is equivalent to \ilc{b} and \ilc{right}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        B second;

        /**
        \brief
        The first value.

        \details
        This is equivalent to \ilc{first} and \ilc{left}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        A &a;

        /**
        \brief
        The second value.

        \details
        This is equivalent to \ilc{second} and \ilc{right}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        B &b;

        /**
        \brief
        The first value.

        \details
        This is equivalent to \ilc{first} and \ilc{a}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        A &left;

        /**
        \brief
        The second value.

        \details
        This is equivalent to \ilc{second} and \ilc{b}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        B &right;

        /**
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
        template<typename OA, typename OB>
        Pair(const OA &first, const OB &second);

        /**
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
        template<typename OA, typename OB>
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
        \param other
        The pair to copy from.

        \brief
        Constructs a new instance from another pair.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        template<typename OA, typename OB>
        Pair(const Pair<OA, OB> &other);

        /**
        \param other
        The pair to move from.

        \brief
        Constructs a new instance from another pair.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        template<typename OA, typename OB>
        Pair(Pair<OA, OB> &&other);
        
        /**
        \tparam N
        The index of the value to get. Must be either \ilc{0} or \ilc{1}.

        \brief 
        Returns the value at the specified index.

        \details
        This function is mainly to allow structured bindings; the data should be accessed using one of the
        public member variables.
        */
        template<sizeType N>
        decltype(auto) get() const;

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
        Pair& operator = (const Pair<A, B> &other);

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
        Pair& operator = (Pair<A, B> &&other);

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
        template<typename OA, typename OB>
        Pair& operator = (const Pair<OA, OB> &other);

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
        template<typename OA, typename OB>
        Pair& operator = (Pair<OA, OB> &&other);
    };

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
    template<typename OA, typename OB>
    Pair<A, B>::Pair(const OA &first, const OB &second) :
        first(first),
        second(second),
        a(this->first),
        b(this->second),
        left(this->first),
        right(this->second)
    {}

    template<typename A, typename B>
    template<typename OA, typename OB>
    Pair<A, B>::Pair(OA &&first, OB &&second) :
        first(static_cast<OA&&>(first)),
        second(static_cast<OB&&>(second)),
        a(this->first),
        b(this->second),
        left(this->first),
        right(this->second)
    {}

    template<typename A, typename B>
    Pair<A, B>::Pair(const Pair<A, B> &other) : Pair(other.first, other.second)
    {}

    template<typename A, typename B>
    Pair<A, B>::Pair(Pair<A, B> &&other) : Pair(static_cast<A&&>(other.first), static_cast<B&&>(other.second))
    {}
    
    template<typename A, typename B>
    template<typename OA, typename OB>
    Pair<A, B>::Pair(const Pair<OA, OB> &other) : Pair(other.first, other.second)
    {}

    template<typename A, typename B>
    template<typename OA, typename OB>
    Pair<A, B>::Pair(Pair<OA, OB> &&other) : Pair(static_cast<A&&>(other.first), static_cast<B&&>(other.second))
    {}
    
    ///\cond

    template<typename A, typename B>
    template<sizeType N>
    decltype(auto) Pair<A, B>::get() const
    {
        static_assert(N == 0 || N == 1);

        if constexpr (N == 0)
            return first;
        else
            return second;
    }
    
    ///\endcond

    template<typename A, typename B>
    Pair<A, B>& Pair<A, B>::operator = (const Pair<A, B> &other)
    {
        first = other.first;
        second = other.second;

        return *this;
    }

    template<typename A, typename B>
    Pair<A, B>& Pair<A, B>::operator = (Pair<A, B> &&other)
    {
        first = static_cast<A&&>(other.first);
        second = static_cast<B&&>(other.second);

        return *this;
    }

    template<typename A, typename B>
    template<typename OA, typename OB>
    Pair<A, B>& Pair<A, B>::operator = (const Pair<OA, OB> &other)
    {
        first = other.first;
        second = other.second;

        return *this;
    }

    template<typename A, typename B>
    template<typename OA, typename OB>
    Pair<A, B>& Pair<A, B>::operator = (Pair<OA, OB> &&other)
    {
        first = static_cast<OA&&>(other.first);
        second = static_cast<OB&&>(other.second);

        return *this;
    }
    
    template<typename A, typename B>
    Pair<A, B> pair(A &&first, B &&second)
    {
        return Pair<A, B>(static_cast<A&&>(first), static_cast<B&&>(second));
    }
}

///\cond
//This exists to enable structured bindings
namespace std
{
    template<typename A, typename B>
    struct tuple_size<nou::Pair<A, B>> : std::integral_constant<std::size_t, 2> {};

    template<typename A, typename B>
    struct tuple_element<0, nou::Pair<A, B>>
    {
        using type = A;
    };

    template<typename A, typename B>
    struct tuple_element<1, nou::Pair<A, B>>
    {
        using type = B;
    };
}
///\endcond */

#endif