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
The main class in this component, <code>nou::Pair</code> behaves somewhat like <code>nou::Tuple</code> that
stores two values. 

Accessing the values stored in a pair is done through the (public) member variables 
<code>\link nou::Pair::first first\endlink</code> and <code>\link nou::Pair::second second\endlink</code>. 
The class also supports structural bindings.

<second>Example:</second>
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
        This is equivalent to <code>first</code> and <code>left</code>.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        A first;

        /**
        \brief
        The second value.

        \details
        This is equivalent to <code>second</code> and <code>right</code>.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        B second;

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
        The index of the value to get. Must be either <code>0</code> or <code>1</code>.

        \brief 
        Returns the value at the specified index.

        \details
        This function is mainly to allow structured bindings; the data should be accessed using one of the
        public member variables.
        */
        //template<sizeType N>
        //decltype(auto) get() const;

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
    Constructs a new <code>nou::Pair</code> instance from two l-value references.

    \details
    This function can be used to write cleaner code, because it does not require the usage of explicit
    template parameters.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    //template<typename A, typename B>
    //Pair<A, B> pair(const A &first, const B &second);

    ///**
    //\param first
    //The first value.

    //\param second
    //The second value.

    //\brief
    //Constructs a new <code>nou::Pair</code> instance from one l-value reference and one r-value reference.

    //\details
    //This function can be used to write cleaner code, because it does not require the usage of explicit
    //template parameters.

    //\author  Lukas Reichmann
    //\version 1.1.0.0
    //\since   1.1.0.0
    //*/
    //template<typename A, typename B>
    //Pair<A, B> pair(const A &first, B &&second);

    ///**
    //\param first
    //The first value.

    //\param second
    //The second value.

    //\brief
    //Constructs a new <code>nou::Pair</code> instance from one r-value reference and one l-value reference.

    //\details
    //This function can be used to write cleaner code, because it does not require the usage of explicit
    //template parameters.

    //\author  Lukas Reichmann
    //\version 1.1.0.0
    //\since   1.1.0.0
    //*/
    //template<typename A, typename B>
    //Pair<A, B> pair(A &&first, const B &second);

    /**
    \param first
    The first value.

    \param second
    The second value.

    \brief
    Constructs a new <code>nou::Pair</code> instance from two r-value references.

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
        second(second)
    {}

    template<typename A, typename B>
    template<typename OA, typename OB>
    Pair<A, B>::Pair(OA &&first, OB &&second) :
        first(static_cast<OA&&>(first)),
        second(static_cast<OB&&>(second))
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
    /*
    template<typename A, typename B>
    template<sizeType N>
    decltype(auto) Pair<A, B>::get() const
    {
        static_assert(N == 0 || N == 1);

        if constexpr (N == 0)
            return first;
        else
            return second;
    }*/
    
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
        first = static_cast<B&&>(other.second);

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
    

/*
    template<typename A, typename B>
    Pair<A, B> pair(const A &first, const B &second)
    {
        return Pair<A, B>(first, second);
    }

    template<typename A, typename B>
    Pair<A, B> pair(const A &first, B &&second)
    {
        return Pair<A, B>(first, static_cast<B&&>(second));
    }

    template<typename A, typename B>
    Pair<A, B> pair(A &&first, const B &second)
    {
        return Pair<A, B>(static_cast<A&&>(first), second);
    }
*/
    template<typename A, typename B>
    Pair<A, B> pair(A &&first, B &&second)
    {
        return Pair<A, B>(static_cast<A&&>(first), static_cast<B&&>(second));
    }
}
/*
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