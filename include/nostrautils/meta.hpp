#ifndef NOU_META_HPP
#define NOU_META_HPP

/**
\file nostrautils/meta.hpp

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0

\brief
A component that provides various meta functions.

\details
The functions in this component can be grouped in three groups:
<ul>
    <li>
        type information: Functions that provide information about a type. These functions build the basis of
        the \link typeinfo typeinfo.hpp\endlink component.
    </li>
    <li>
        type modification: Functions that take in a type and modify that type in some way (e.g. they add
        \ilc{const}).
    </li>
    <li>
        logic: Functions that are provide some sort of logic for meta functions (e.g. comparing types, if
        statements).
    </li>
</ul>

In addition to these groups, all meta functions can be divided into two types: those that return types and
those that return values.
For each of these two types, the result is stored in fixed members: the result type of a function that
returns a type is stored in a public member alias \ilc{Type} and the result value of a function that returns
a value is stored in a public static member constant \ilc{value}. The functions that return a type also each
have a global alias name in the namespace \ilc{nou} called \ilc{\<name of the meta function>Type} (e.g. the
global alias of \ilc{AddConst<T>} is \ilc{AddConstType<T>}).

\par_example
\code{.cpp}
using constInt32 = nou::AddConstType<nou::int32>; //constInt32 is const nou::int32

template<typename T>
void func(const T &t)
{
    if constexpr (nou::AreSame<nou::int32, T>::value)
    {
        //will be executed if T is nou::int32
    }
}
\endcode

For a more detailed example, see \link meta.ex.cpp here\endlink.
*/

/**
\example meta.ex.cpp

An example that demonstrates the usage of the declval component.
*/

#ifndef NOU_TYPES_HPP
#    include "nostrautils/types.hpp"
#endif

#include <type_traits>

namespace nou
{
    /**
    \tparam T
    The type of the constant.

    \return
    \ilc{T}.

    \brief
    A meta function that stores the type that was passed to it.

    \details
    There is no \ilc{IdentityType} alias.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    struct Identity
    {
        /**
        \brief The stored type.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        using Type = T;
    };

    /**
    \tparam T
    The type of the constant.

    \tparam VALUE
    The value of the constant.

    \return
    Both the type of the value (in \ilc{type}) and the value itself (in \ilc{value}).

    \brief
    A meta function that can store a single constant of a single type.

    \details
    This function is somewhat special, because it returns a type and a value. In addition to that, there is
    no \ilc{ConstantType} alias.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T, T VALUE>
    struct Constant : Identity<T>
    {
        /**
        \brief The value of the constant.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        static constexpr typename Identity<T>::Type value = VALUE;
    };

    /**
    \tparam B
    The value of the boolean. Either \ilc{true} of \ilc{false}.

    \return
    \ilc{B}.

    \brief
    A constant that can exclusively store value of the type \ilc{nou::boolean}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<nou::boolean B>
    using BooleanConstant = Constant<nou::boolean, B>;

    /**
    \return
    \ilc{true}.

    \brief
    A constant that is always \ilc{true}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    using TrueConstant = BooleanConstant<true>;

    /**
    \return
    \ilc{false}.

    \brief
    A constant that is always \ilc{false}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    using FalseConstant = BooleanConstant<false>;

    /**
    \tparam V
    The boolean value that decides which type will be chosen.

    \tparam A
    The first type that may be chosen.

    \tparam B
    The second type that may be chosen.

    \return
    \ilc{A} if \ilc{V} is \ilc{true}, and \ilc{B} if \ilc{V} is \ilc{false}.

    \brief
    A function that returns one of two types depending on a passed boolean.

    \details
    Expressing this function with pseudo code would look something like this:

    \code{.cpp}
    if(V)
        return A
    else
        return B
    \endcode

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<nou::boolean V, typename A, typename B>
    struct If : Identity<A>
    {};

    ///\cond
    template<typename A, typename B>
    struct If<false, A, B> : Identity<B>
    {};
    ///\endcond

    /**
    \tparam V
    The boolean value that decides which type will be chosen.

    \tparam A
    The first type that may be chosen.

    \tparam B
    The second type that may be chosen.

    \brief
    The result of \ilc{nou::If}.
    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<nou::boolean V, typename A, typename B>
    using IfType = typename If<V, A, B>::Type;

    namespace internal
    {
        /**
        \tparam I
        The index.
        \tparam T
        The first type in the parameter pack.
                 \tparam ARGS
        The rest of the parameter pack.

        \return
        The type in the parameter pack at the index \ilc{I}.

        \brief
        Returns the type in the parameter pack with the index \ilc{I}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        template<nou::uint32 I, typename T, typename... ARGS>
        struct Get : Identity<typename Get<I - 1, ARGS...>::Type>
        {};

        ///\cond
        template<typename T, typename... ARGS>
        struct Get<0, T, ARGS...> : Identity<T>
        {};
        ///\endcond
    } // namespace internal

    /**
    \tparam I
    The index.

    \tparam ARGS
    The parameter pack.

    \return
    The type in the parameter \ilc{ARGS} pack with the index \ilc{I}.

    \brief
    Returns the type in the parameter \ilc{ARGS} pack with the index \ilc{I}.

    \details
    \ilc{I} must be smaller than \ilc{sizeof...(ARGS)}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<nou::uint32 I, typename... ARGS>
    struct Get : Identity<typename internal::Get<I, ARGS...>::Type>
    {
        static_assert(I < sizeof...(ARGS));
    };

    /**
    \tparam I
    The index.

    \tparam ARGS
    The parameter pack.

    \brief
    The result of \ilc{nou::Get}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<nou::uint32 I, typename... ARGS>
    using GetType = typename Get<I, ARGS...>::Type;

    /**
    \tparam B
    The boolean value that decides if there is a return type.

    \tparam T
    The type that may be returned.

    \return
    \ilc{T} if \ilc{B} is \ilc{true}.

    \brief
    Returns \ilc{T} if \ilc{B} is \ilc{true}, otherwise there will be no return type.

    \details
    This function can be used to use <a href="https://en.cppreference.com/w/cpp/types/enable_if">
    SFINAE</a>.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<nou::boolean B, typename T = void>
    struct EnableIf : Identity<T>
    {};

    ///\cond
    template<typename T>
    struct EnableIf<false, T>
    {};
    ///\endcond

    /**
    \tparam B
    The boolean value that decides if there is a return type.

    \tparam T
    The type that may be returned.

    \brief
    The result of \ilc{nou::EnableIf}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<nou::boolean B, typename T = void>
    using EnableIfType = typename EnableIf<B, T>::Type;

    /**
    \tparam A
    The first type to check.

    \tparam B
    The second type to check.

    \tparam ARGS
    The additional types to check.

    \tparam T
    The type that may be returned.

    \return
    \ilc{nou::TrueConstant}, if the types \ilc{A}, \ilc{B} (and if given, \ilc{ARGS}) are the same,
    \ilc{nou::FalseConstant} if not.

    \brief
    Checks whether the passed types are the same.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename A, typename B, typename... T2>
    struct AreSame : FalseConstant
    {};

    ///\cond
    template<typename A, typename... ARGS>
    struct AreSame<A, A, ARGS...> : AreSame<A, ARGS...>
    {};

    template<typename A, typename B>
    struct AreSame<A, B> : FalseConstant
    {};

    template<typename T>
    struct AreSame<T, T> : TrueConstant
    {};
    ///\endcond

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is an array type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is an array.
    */
    template<typename T>
    struct IsArray : BooleanConstant<std::is_array_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is \ilc{void}, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is \ilc{void}.
    */
    template<typename T>
    struct IsVoid : AreSame<T, void>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is \ilc{nou::nullptrType}, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is \ilc{nou::nullptrType}.
    */
    template<typename T>
    struct IsNullptrType : AreSame<T, decltype(nullptr)>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is an integral type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is an integral type.
    */
    template<typename T>
    struct IsIntegral : BooleanConstant<std::is_integral_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is a floating point type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is a floating point type.
    */
    template<typename T>
    struct IsFloatingPoint : BooleanConstant<std::is_floating_point_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is an enumeration type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is an enumeration type.
    */
    template<typename T>
    struct IsEnum : BooleanConstant<std::is_enum_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is a union type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is a union type.
    */
    template<typename T>
    struct IsUnion : BooleanConstant<std::is_union_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is a class type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is a class type.
    */
    template<typename T>
    struct IsClass : BooleanConstant<std::is_class_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is a function type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is a function type.
    */
    template<typename T>
    struct IsFunction : BooleanConstant<std::is_function_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is a pointer type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is a pointer type.
    */
    template<typename T>
    struct IsPointer : BooleanConstant<std::is_pointer_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is a l-value reference type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is a l-value reference type.
    */
    template<typename T>
    struct IsLValueReference : BooleanConstant<std::is_lvalue_reference_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is a r-value reference type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is a r-value reference type.
    */
    template<typename T>
    struct IsRValueReference : BooleanConstant<std::is_rvalue_reference_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is a member object pointer type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is a member object pointer type.
    */
    template<typename T>
    struct IsMemberObjectPointer : BooleanConstant<std::is_member_object_pointer_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is a member function pointer type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is a member function pointer type.
    */
    template<typename T>
    struct IsMemberFunctionPointer : BooleanConstant<std::is_member_function_pointer_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is a fundamental type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is a fundamental type.
    */
    template<typename T>
    struct IsFundamental : BooleanConstant<std::is_fundamental_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is an arithmetic type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether an arithmetic is a fundamental type.
    */
    template<typename T>
    struct IsArithmetic : BooleanConstant<std::is_arithmetic_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is a scalar type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is a scalar type.
    */
    template<typename T>
    struct IsScalar : BooleanConstant<std::is_scalar_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is an object type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is an object type.
    */
    template<typename T>
    struct IsObject : BooleanConstant<std::is_object_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is a compound type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is a compound type.
    */
    template<typename T>
    struct IsCompound : BooleanConstant<std::is_compound_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is a reference type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is a fundamental type.
    */
    template<typename T>
    struct IsReference : BooleanConstant<std::is_reference_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is a member pointer type, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is a fundamental type.
    */
    template<typename T>
    struct IsMemberPointer : BooleanConstant<std::is_member_pointer_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is declared \ilc{const}, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is declared \ilc{const}.
    */
    template<typename T>
    struct IsConst : BooleanConstant<std::is_const_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is declared \ilc{volatile}, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is declared \ilc{volatile}.
    */
    template<typename T>
    struct IsVolatile : BooleanConstant<std::is_volatile_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is trivial, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is trivial.
    */
    template<typename T>
    struct IsTrivial : BooleanConstant<std::is_trivial_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is trivially copyable, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is trivially copyable.
    */
    template<typename T>
    struct IsTrivialCopyable : BooleanConstant<std::is_trivially_copyable_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} has a standard layout, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type has a standard layout.
    */
    template<typename T>
    struct IsStandardLayout : BooleanConstant<std::is_standard_layout_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is empty, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is empty.
    */
    template<typename T>
    struct IsEmpty : BooleanConstant<std::is_empty_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is polymorphic, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is polymorphic.
    */
    template<typename T>
    struct IsPolymorphic : BooleanConstant<std::is_polymorphic_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is abstract, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is abstract.
    */
    template<typename T>
    struct IsAbstract : BooleanConstant<std::is_abstract_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is final, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is final.
    */
    template<typename T>
    struct IsFinal : BooleanConstant<std::is_final_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is aggregate, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is aggregate.
    */
    template<typename T>
    struct IsAggregate : BooleanConstant<std::is_aggregate_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is signed, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is signed.
    */
    template<typename T>
    struct IsSigned : BooleanConstant<std::is_signed_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is unsigned, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is unsigned.
    */
    template<typename T>
    struct IsUnsigned : BooleanConstant<std::is_unsigned_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \tparam ARGS
    The parameter types to construct from.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is constructible with instances of the types \ilc{ARGS},
    \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is constructible.
    */
    template<typename T, typename... ARGS>
    struct IsConstructible : BooleanConstant<std::is_constructible_v<T, ARGS...>>
    {};

    /**
    \tparam T
    The type to check.

    \tparam ARGS
    The parameter types to construct from.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is trivially constructible with instances of the types \ilc{ARGS},
    \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is trivially constructible.
    */
    template<typename T, typename... ARGS>
    struct IsTriviallyConstructible : BooleanConstant<std::is_trivially_constructible_v<T, ARGS...>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is default constructible, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is default constructible.
    */
    template<typename T>
    struct IsDefaultConstructible : BooleanConstant<std::is_default_constructible_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is trivially default constructible, \ilc{nou::FalseConstant} if it
    not.

    \brief
    Checks whether a type is trivially default constructible.
    */
    template<typename T>
    struct IsTriviallyDefaultConstructible : BooleanConstant<std::is_trivially_default_constructible_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is copy constructible, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is copy constructible.
    */
    template<typename T>
    struct IsCopyConstructible : BooleanConstant<std::is_copy_constructible_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is trivially copy constructible, \ilc{nou::FalseConstant} if it
    not.

    \brief
    Checks whether a type is trivially copy constructible.
    */
    template<typename T>
    struct IsTriviallyCopyConstructible : BooleanConstant<std::is_trivially_copy_constructible_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is move constructible, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is move constructible.
    */
    template<typename T>
    struct IsMoveConstructible : BooleanConstant<std::is_move_constructible_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is trivially move constructible, \ilc{nou::FalseConstant} if it
    not.

    \brief
    Checks whether a type is trivially move constructible.
    */
    template<typename T>
    struct IsTriviallyMoveConstructible : BooleanConstant<std::is_trivially_move_constructible_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{FROM} is assignable to \ilc{TO}, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is assignable to another.
    */
    template<typename TO, typename FROM>
    struct IsAssignable : BooleanConstant<std::is_assignable_v<TO, FROM>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{FROM} is trivially assignable to \ilc{TO}, \ilc{nou::FalseConstant} if it
    not.

    \brief
    Checks whether a type is trivially assignable to another.
    */
    template<typename TO, typename FROM>
    struct IsTriviallyAssignable : BooleanConstant<std::is_trivially_assignable_v<TO, FROM>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is copy assignable, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is copy assignable.
    */
    template<typename T>
    struct IsCopyAssignable : BooleanConstant<std::is_copy_assignable_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is trivially copy assignable, \ilc{nou::FalseConstant} if it
    not.

    \brief
    Checks whether a type is trivially copy assignable.
    */
    template<typename T>
    struct IsTriviallyCopyAssignable : BooleanConstant<std::is_trivially_copy_assignable_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is move assignable, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is move assignable.
    */
    template<typename T>
    struct IsMoveAssignable : BooleanConstant<std::is_move_assignable_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is trivially move assignable, \ilc{nou::FalseConstant} if it
    not.

    \brief
    Checks whether a type is trivially move assignable.
    */
    template<typename T>
    struct IsTriviallyMoveAssignable : BooleanConstant<std::is_trivially_move_assignable_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is destructible, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is destructible.
    */
    template<typename T>
    struct IsDestructible : BooleanConstant<std::is_destructible_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} is trivially destructible, \ilc{nou::FalseConstant} if it
    not.

    \brief
    Checks whether a type is trivially destructible.
    */
    template<typename T>
    struct IsTriviallyDestructible : BooleanConstant<std::is_trivially_destructible_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{T} has a virtual destructor, \ilc{nou::FalseConstant} if it not.

    \brief
    Checks whether a type is trivially destructible.
    */
    template<typename T>
    struct HasVirtualDestructor : BooleanConstant<std::has_virtual_destructor_v<T>>
    {};

    /**
    \tparam T
    The type to check.

    \return
    The amount of dimensions of an array (and \ilc{0} for any other type).

    \brief
    Returns the amount of dimensions of an array.
    */
    template<typename T>
    struct ArrayRank : Constant<decltype(std::rank_v<T>), std::rank_v<T>>
    {};

    /**
    \tparam BASE
    The base type to check.

    \tparam DERIVED
    The derived type to check.

    \return
    \ilc{nou::TrueConstant}, if \ilc{B} is a base class of \ilc{D}, \ilc{nou::FalseConstant} if it not.

    \brief
    Returns whether a type is a base class of another.
    */
    template<typename BASE, typename DERIVED>
    struct IsBaseOf : BooleanConstant<std::is_base_of_v<BASE, DERIVED>>
    {};

    /**
    \tparam FROM
    The base type to convert from.

    \tparam TO
    The derived type to convert to.

    \return
    \ilc{nou::TrueConstant}, if \ilc{B} is a base class of \ilc{D}, \ilc{nou::FalseConstant} if it not.

    \brief
    Returns whether a type is a base class of another.
    */
    template<typename FROM, typename TO>
    struct IsConvertible : BooleanConstant<std::is_convertible_v<FROM, TO>>
    {};

    /**
    \tparam F
    The function to invoke.

    \tparam ARGS
    The parameter types to invoke \ilc{F} with.

    \return
    \ilc{nou::TrueConstant}, if \ilc{F} is invokable with the parameters of the types \ilc{ARGS},
    \ilc{nou::FalseConstant} if it not.

    \brief
    Returns whether a function is invokable with certain parameters.
    */
    template<typename F, typename... ARGS>
    struct IsInvokable : BooleanConstant<std::is_invocable_v<F, ARGS...>>
    {};

    /**
    \tparam F
    The function to invoke.

    \tparam ARGS
    The parameter types to invoke \ilc{F} with.

    \return
    \ilc{nou::TrueConstant}, if \ilc{F} is invokable with the parameters of the types \ilc{ARGS} and returns
    a result that is convertible to \ilc{R}, \ilc{nou::FalseConstant} if it not.

    \brief
    Returns whether a function is invokable with certain parameters and its return value is convertible to
    a certain type.
    */
    template<typename R, typename F, typename... ARGS>
    struct IsInvokableR : BooleanConstant<std::is_invocable_r_v<R, F, ARGS...>>
    {};

    /**
    \tparam T
    The type to modify.

    \return
    \ilc{T} without the \ilc{const} modifier.

    \brief
    Returns the passed type without the \ilc{const} modifier.
    */
    template<typename T>
    struct RemoveConst : Identity<std::remove_const_t<T>>
    {};

    /**
    \tparam T
    The type to modify.

    \brief
    The result of \ilc{nou::RemoveConst}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    using RemoveConstType = typename RemoveConst<T>::Type;

    /**
    \tparam T
    The type to modify.

    \return
    \ilc{T} without the \ilc{volatile} modifier.

    \brief
    Returns the passed type without the \ilc{volatile} modifier.
    */
    template<typename T>
    struct RemoveVolatile : Identity<std::remove_volatile_t<T>>
    {};

    /**
    \tparam T
    The type to modify.

    \brief
    The result of \ilc{nou::RemoveVolatile}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    using RemoveVolatileType = typename RemoveVolatile<T>::Type;

    /**
    \tparam T
    The type to modify.

    \return
    \ilc{T} with the \ilc{const} modifier added.

    \brief
    Returns the passed type with the \ilc{const} modifier added.
    */
    template<typename T>
    struct AddConst : Identity<std::add_const_t<T>>
    {};

    /**
    \tparam T
    The type to modify.

    \brief
    The result of \ilc{nou::AddConst}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    using AddConstType = typename AddConst<T>::Type;

    /**
    \tparam T
    The type to modify.

    \return
    \ilc{T} with the \ilc{volatile} modifier added.

    \brief
    Returns the passed type with the \ilc{volatile} modifier added.
    */
    template<typename T>
    struct AddVolatile : Identity<std::add_volatile_t<T>>
    {};

    /**
    \tparam T
    The type to modify.

    \brief
    The result of \ilc{nou::AddVolatile}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    using AddVolatileType = typename AddVolatile<T>::Type;

    /**
    \tparam T
    The type to modify.

    \return
    \ilc{T} as a non-reference.

    \brief
    Returns the passed type as a non-reference.
    */
    template<typename T>
    struct RemoveReference : Identity<std::remove_reference_t<T>>
    {};

    /**
    \tparam T
    The type to modify.

    \brief
    The result of \ilc{nou::RemoveReference}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    using RemoveReferenceType = typename RemoveReference<T>::Type;

    /**
    \tparam T
    The type to modify.

    \return
    \ilc{T} as L-value reference modifier added.

    \brief
    Returns the passed type as an L-value reference.
    */
    template<typename T>
    struct AddLValueReference : Identity<std::add_lvalue_reference_t<T>>
    {};

    /**
    \tparam T
    The type to modify.

    \brief
    The result of \ilc{nou::AddLValueReference}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    using AddLValueReferenceType = typename AddLValueReference<T>::Type;

    /**
    \tparam T
    The type to modify.

    \return
    \ilc{T} as R-value reference modifier added.

    \brief
    Returns the passed type as an R-value reference.
    */
    template<typename T>
    struct AddRValueReference : Identity<std::add_rvalue_reference_t<T>>
    {};

    /**
    \tparam T
    The type to modify.

    \brief
    The result of \ilc{nou::AddRValueReference}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    using AddLValueReferenceType = typename AddLValueReference<T>::Type;

    /**
    \tparam T
    The type to modify.

    \return
    The pointer type of \ilc{T}.

    \brief
    Returns the pointer type of \ilc{T}.
    */
    template<typename T>
    struct AddPointer : Identity<T *>
    {};

    /**
    \tparam T
    The type to modify.

    \brief
    The result of \ilc{nou::AddPointer}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    using AddPointerType = typename AddPointer<T>::Type;

    /**
    \tparam T
    The type to modify.

    \return
    The type that \ilc{T} points to.

    \brief
    Returns type that a pointer points to.

    \details
    If \ilc{T} is not a pointer type, the result will be \ilc{T}.
    */
    template<typename T>
    struct RemovePointer : Identity<T>
    {};

    ///\cond
    template<typename T>
    struct RemovePointer<T *> : Identity<T>
    {};
    ///\endcond

    /**
    \tparam T
    The type to modify.

    \brief
    The result of \ilc{nou::RemovePointer}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    using RemovePointerType = typename RemovePointer<T>::Type;

    /**
    \tparam F
    The callable type.

    \return
    The result of calling a callable of type \ilc{F} with the parameters of the types \ilc{ARGS}.

    \brief
    Returns the result of calling a callable of type \ilc{F} with the parameters of the types \ilc{ARGS}.
    */
    template<typename F, typename... ARGS>
    struct InvokeResult : Identity<std::invoke_result<F, ARGS...>>
    {};

    /**
    \tparam T
    The type to modify.

    \brief
    The result of \ilc{nou::InvokeResult}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename F, typename... ARGS>
    using InvokeResultType = typename InvokeResult<F, ARGS...>::Type;

    /**
    \tparam ARGS
    Any types, will be ignored.

    \return
    \ilc{void}.

    \brief
    Always returns \ilc{void}.
    */
    template<typename... ARGS>
    struct Void : Identity<void>
    {};

    /**
    \tparam ARGS
    Any types, will be ignored.

    \brief
    The result of \ilc{nou::Void}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename... ARGS>
    using VoidType = typename Void<ARGS...>::Type;

} // namespace nou

#endif
