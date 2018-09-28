#ifndef NOU_NOT_NULL_HPP
#define NOU_NOT_NULL_HPP

/**
\file nostrautils/notnull.hpp

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0

\brief
A component that provides a wrapper for (smart-) pointer types that prevents the wrapped pointer to be set to
\ilc{nullptr}.

\details
If a user attempts to assign a \ilc{nullptr} to an instance of \ilc{nou::NotNull}, that instance will either
trigger a compile error (whenever possible) or a runtime error as soon as the assignment is attempted.

Other than that, instances of \ilc{nou::NotNull} will behave like a regular pointer.

This behavior can optimize code, because it does not require additional, redundant checks if a pointer is
\ilc{nullptr} - except for the initial check of course. In addition to that, it makes finding bugs far
easier, since whenever a \ilc{nullptr} is set, the class will trigger an error when the pointer is set and
not when it is accessed.

\par_example
\code{.cpp}
int main()
{
    nou::NotNull<nou::int32*> iPtr1 = nullptr; //results in a compile error

    nou::int32 *ptr = nullptr;
    nou::NotNull<nou::int32*> iPtr2 = ptr; //results in a runtime error
}
\endcode

For a more detailed example, see \link notnull.ex.cpp here\endlink.

\ilc{nou::NotNull} supports \ilc{constexpr} whenever possible. If \ilc{nullptr} is assigned to an instance in
a case that would usually trigger a runtime error, a compile error is triggered instead. In that case, the
compiler will most likely fail with a message that says something like: "expression did not evaluate to a
constant" (this is the error that MSVC prints). This is the intended behavior.

\note
\ilc{nou::NotNull} on its own is not a smart pointer - it does not prevent a user from copying pointers nor
does it any memory management.
*/

/**
\example notnull.ex.cpp

An example that demonstrates the usage of the notnull component.
*/

#ifndef NOU_TYPES_HPP
#    include "nostrautils/types.hpp"
#endif

#ifndef NOU_DECLVAL_HPP
#    include "nostrautils/declval.hpp"
#endif

#ifndef NOU_META_HPP
#    include "nostrautils/meta.hpp"
#endif

#include <cstdlib>
#include <type_traits>
#include <utility>

namespace nou
{
    namespace internal
    {
        /**
        \tparam T
        The type to convert.

        \return
        \ilc{const T}, even if \p T is a pointer type.

        \brief
        A meta function that adds \ilc{const} to a type, even if that type is a pointer.

        \details
        The 'return type' is stored in \ilc{Type}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        template<typename T>
        struct NotNullMakeConstType
        {
            /**
            \brief
            The 'return type' of this meta function.

            \author  Lukas Reichmann
            \version 1.1.0.0
            \since   1.1.0.0
            */
            using Type = const T;
        };

        ///\cond
        // No need to have this specialization in the documentation
        template<typename T>
        struct NotNullMakeConstType<T *>
        {
            using Type = const T *;
        };
        ///\endcond

        /**
        \tparam T
        The type to convert.

        \return
        \p T& if \p T is not a pointer type, \p T if it is.

        \brief
        A meta function that converts \p T into \p T& if \p T is not a pointer, or leaves \p T as-is if it is
        not a pointer type.

        \details
        The 'return type' is stored in \ilc{Type}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        template<typename T>
        struct NotNullMakeRefType : Identity<T &>
        {};

        ///\cond
        // No need to have this specialization in the documentation
        template<typename T>
        struct NotNullMakeRefType<T *> : Identity<T *>
        {};
        ///\endcond
    } // namespace internal

    /**
    \tparam T
    The pointer type that this class wraps around. Not that, if e.g. this class should emulate an
    \ilc{nou::int32*}, this type needs to be \ilc{nou::int32*} as well, not just \ilc{nou::int32}.

    \brief
    A class that wraps around a pointer and prevents that pointer from becoming \ilc{nullptr}.

    \details
    See notnull.hpp for a full description.

    \see notnull.hpp

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    class NotNull final
    {
    public:
        /**
        \brief
        The full type that is being wrapped around.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        using Type = T;

        /**
        \brief
        The same type as \ilc{Type}, but \ilc{const}.

        \details
        \par_example
        If \ilc{Type} is \ilc{nou::int32*}, then \ilc{ConstType} is
        \ilc{const nou::int32*}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        using ConstType = typename internal::NotNullMakeConstType<Type>::Type;

        /**
        \brief
        Depending on the \ilc{ConstType}, this is \ilc{ConstType&} or just
        \ilc{ConstType} (see details).

        \details
        \par_example
        If \ilc{ConstType} is a pointer type, then \ilc{ConstTypeRef} is
        \ilc{ConstType}. If \ilc{ConstType} is any other type, then
        \ilc{ConstTypeRef} is \ilc{ConstType&}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        using ConstTypeRef = typename internal::NotNullMakeRefType<ConstType>::Type;

    private:
        /**
        \brief
        The pointer that is being wrapped.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        Type m_ptr;

        /**
        \brief
        Calls \ilc{\::abort()} if \ilc{m_ptr} is \ilc{nullptr}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr void checkNull() const;

    public:
        /**
        \brief
        The type that the wrapped pointer points to.

        \details
        \par_example
        \ilc{nou::NotNull<nou::int32*>::RawType} is \ilc{nou::int32}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        using RawType = decltype(*m_ptr);

        constexpr NotNull() = default;

        NotNull(nullptrType) = delete;

        /**
        \param ptr
        The pointer that will be wrapped. Must not be \ilc{nullptr}.

        \brief
        Constructs a new instance that wraps the passed pointer.

        \details
        If \p ptr is \ilc{nullptr}, \ilc{\::abort()} will be called.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr NotNull(Type ptr);

        /**
        \return
        The wrapped pointer as-is.

        \brief
        Returns the wrapped pointer as-is.

        \details
        This function is supposed to enable compatibility with other functions that required a raw pointer.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        inline Type rawPtr();

        /**
        \return
        The wrapped pointer as-is.

        \brief
        Returns the wrapped pointer as-is.

        \details
        This function is supposed to enable compatibility with other functions that required a raw pointer.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr ConstTypeRef rawPtr() const;

        /**
        \return
        A reference to the wrapped pointer.

        \brief
        Calls the arrow operator as if it was called directly on the wrapped pointer.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        inline Type &operator->();

        /**
        \return
        A reference to the wrapped pointer.

        \brief
        Calls the arrow operator as if it was called directly on the wrapped pointer.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr ConstTypeRef operator->() const;

        /**
        \return
        A reference to the object that the wrapped pointer points to.

        \brief
        Calls the indirection (asterisk) operator as if it was called directly on the wrapped pointer.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        inline RawType &operator*();

        /**
        \return
        A reference to the object that the wrapped pointer points to.

        \brief
        Calls the indirection (asterisk) operator as if it was called directly on the wrapped pointer.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr const RawType &operator*() const;

        /**
        \param offset
        The offset of the object to access. This values is in multiples of the size of the raw type.

        \return
        A reference to the object that is at the address of the wrapped pointer plus the offset.

        \brief
        Calls the subscript operator as if it was called directly on the wrapped pointer.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        inline RawType &operator[](ptrdiffType offset);

        /**
        \param offset
        The offset of the object to access. This values is in multiples of the size of the raw type.

        \return
        A reference to the object that is at the address of the wrapped pointer plus the offset.

        \brief
        Calls the subscript operator as if it was called directly on the wrapped pointer.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr const RawType &operator[](ptrdiffType offset) const;

        /**
        \param other
        The pointer to compare to.

        \return
        \ilc{true}, if the wrapped pointer of the calling instance and the wrapped pointer of the passed
        instance are equal, \ilc{false} if not.

        \brief
        Compares two pointers on equality.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator==(const NotNull &other) const;

        /**
        \param other
        The pointer to compare to.

        \return
        \ilc{true}, if the wrapped pointer of the calling instance is equal to the passed pointer,
        \ilc{false} if not.

        \brief
        Compares two pointers on equality.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator==(ConstTypeRef other) const;

        /**
        \param other
        The pointer to compare to.

        \return
        \ilc{false}, if the wrapped pointer of the calling instance and the wrapped pointer of the passed
        instance are equal, \ilc{true} if not.

        \brief
        Compares two pointers on inequality.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator!=(const NotNull &other) const;

        /**
        \param other
        The pointer to compare to.

        \return
        \ilc{false}, if the wrapped pointer of the calling instance is equal to the passed pointer,
        \ilc{true} if not.

        \brief
        Compares two pointers on inequality.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator!=(ConstTypeRef other) const;

        /**
        \param other
        The pointer to compare to.

        \return
        \ilc{true}, if the wrapped pointer of the calling instance is smaller than the wrapped pointer of the
        passed instance, \ilc{true} if not.

        \brief
        Compares two pointers.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator<(const NotNull &other) const;

        /**
        \param other
        The pointer to compare to.

        \return
        \ilc{true}, if the wrapped pointer of the calling instance is smaller than the passed pointer,
        \ilc{true} if not.

        \brief
        Compares two pointers.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator<(ConstTypeRef other) const;

        /**
        \param other
        The pointer to compare to.

        \return
        \ilc{true}, if the wrapped pointer of the calling instance is larger than the wrapped pointer of the
        passed instance, \ilc{true} if not.

        \brief
        Compares two pointers.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator>(const NotNull &other) const;

        /**
        \param other
        The pointer to compare to.

        \return
        \ilc{true}, if the wrapped pointer of the calling instance is larger than the passed pointer,
        \ilc{true} if not.

        \brief
        Compares two pointers.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator>(ConstTypeRef other) const;

        /**
        \param other
        The pointer to compare to.

        \return
        \ilc{true}, if the wrapped pointer of the calling instance is smaller than or equal to the wrapped
        pointer of the passed instance, \ilc{true} if not.

        \brief
        Compares two pointers.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator<=(const NotNull &other) const;

        /**
        \param other
        The pointer to compare to.

        \return
        \ilc{true}, if the wrapped pointer of the calling instance is smaller than or equal to the passed
        pointer, \ilc{true} if not.

        \brief
        Compares two pointers.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator<=(ConstTypeRef other) const;

        /**
        \param other
        The pointer to compare to.

        \return
        \ilc{true}, if the wrapped pointer of the calling instance is larger than or equal to the wrapped
        pointer of the passed instance, \ilc{true} if not.

        \brief
        Compares two pointers.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator>=(const NotNull &other) const;

        /**
        \param other
        The pointer to compare to.

        \return
        \ilc{true}, if the wrapped pointer of the calling instance is larger than or equal to the
        passed pointer, \ilc{true} if not.

        \brief
        Compares two pointers.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator>=(ConstTypeRef other) const;

        /**
        \return
        \ilc{false}.

        \brief
        Returns whether the wrapped pointer is \ilc{nullptr}, which is obviously always
        \ilc{false}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator!() const;

        /**
        \param other
        The pointer to apply the AND operation with.

        \return
        \ilc{true}.

        \brief
        Applies a logical AND operation on the two pointers.

        \details
        This function always returns \ilc{true}, because, by definition, both operands are never
        \ilc{nullptr}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator&&(const NotNull &other) const;

        /**
        \param other
        The pointer to apply the AND operation with.

        \return
        \ilc{true} if \p other is not \ilc{nullptr} and \ilc{false} if it is.

        \brief
        Applies a logical AND operation on the two pointers.

        \details
        This function only depends on \p other, since the instance that the operator is called on can never
        be \ilc{nullptr}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator&&(ConstTypeRef other) const;

        /**
        \param other
        The pointer to apply the OR operation with.

        \return
        \ilc{true}.

        \brief
        Applies a logical OR operation on the two pointers.

        \details
        This function always returns \ilc{true}, because, per definition, both operands are never
        \ilc{nullptr}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator||(const NotNull &other) const;

        /**
        \param other
        The pointer to apply the OR operation with.

        \return
        \ilc{true}.

        \brief
        Applies a logical OR operation on the two pointers.

        \details
        This function always returns \ilc{true}, because, per definition, the left operand is never
        \ilc{nullptr}.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr boolean operator||(ConstTypeRef other) const;

        /**
        \return
        \ilc{true}.

        \brief
        Returns whether the wrapped pointer is not \ilc{nullptr}, which is obviously always true.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr operator boolean() const;

        /**
        \return
        \ret_selfref_op

        \brief
        Increments the wrapped pointer by one.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        inline NotNull &operator++();

        /**
        \return
        \ret_selfref_op

        \brief
        Decrements the wrapped pointer by one.

        \warning
        For performance reasons, this operator does not check whether the wrapped pointer will be
        \ilc{nullptr} after the decrementation.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        inline NotNull &operator--();

        /**
        \return
        A copy of the instance from before the incrementation took place.

        \brief
        Increments the wrapped pointer by one.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        inline NotNull operator++(int);

        /**
        \return
        A copy of the instance from before the incrementation took place.

        \brief
        Decrements the wrapped pointer by one.

        \warning
        For performance reasons, this operator does not check whether the wrapped pointer will be
        \ilc{nullptr} after the decrementation.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        inline NotNull operator--(int);

        /**
        \param offset
        The value to add to the wrapped pointer.

        \return
        A new instance that is incremented by \p offset.

        \brief
        Increments the wrapped pointer by \p offset and returns the value in a new instance.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        template<typename PointerType,
                 typename = nou::EnableIfType<
                     nou::IsConstructible<NotNull<T>, decltype(m_ptr + nou::declval<PointerType>())>::value>>
        constexpr NotNull operator+(const PointerType &offset) const;

        /**
        \param other
        The pointer to subtract.

        \return
        The value of the wrapped pointer minus the passed one.

        \brief
        Subtracts two pointers from each other.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr ptrdiffType operator-(const NotNull &other) const;

        /**
        \param other
        The pointer to subtract.

        \return
        The value of the wrapped pointer minus the passed one.

        \brief
        Subtracts two pointers from each other.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        constexpr ptrdiffType operator-(ConstTypeRef other) const;

        /**
        \param offset
        The value to subtract from the wrapped pointer.

        \return
        A new instance that is decremented by \p offset.

        \brief
        Decrements the wrapped pointer by \p offset and returns the value in a new instance.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        template<typename PointerType,
                 typename = nou::EnableIfType<
                     nou::IsConstructible<NotNull<T>, decltype(m_ptr - nou::declval<PointerType>())>::value>>
        constexpr NotNull operator-(const PointerType &offset) const;

        NotNull &operator=(nullptrType) = delete;

        /**
        \param ptr
        The pointer that will be wrapped. Must not be \ilc{nullptr}.

        \return
        \ret_selfref_op

        \brief
        Sets the wrapped pointer to the passed one.

        \details
        If \p ptr is \ilc{nullptr}, \ilc{\::abort()} will be called.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        inline NotNull &operator=(Type ptr);

        /**
        \param offset
        The value to add to the wrapped pointer.

        \return
        \ret_selfref_op

        \brief
        Increments the wrapped pointer by \p offset.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        template<typename PointerType>
        inline NotNull &operator+=(const PointerType &offset);

        /**
        \param offset
        The value to subtract from the wrapped pointer.

        \return
        \ret_selfref_op

        \brief
        Decrements the wrapped pointer by \p offset.

        \author  Lukas Reichmann
        \version 1.1.0.0
        \since   1.1.0.0
        */
        template<typename PointerType>
        inline NotNull &operator-=(const PointerType &offset);
    };

    /**
    \param other
    The pointer to compare to.

    \param notNull
    The \ilc{nou::NotNull} instance.

    \return
    \ilc{true}, if the wrapped pointer of \p notNull is equal to \p other, \ilc{false} if not.

    \brief
    Compares two pointers on equality.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    constexpr boolean operator==(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

    /**
    \param other
    The pointer to compare to.

    \param notNull
    The \ilc{nou::NotNull} instance.

    \return
    \ilc{false}, if the wrapped pointer of \p notNull is equal to \p other, \ilc{true} if not.

    \brief
    Compares two pointers on inequality.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    constexpr boolean operator!=(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

    /**
    \param other
    The pointer to compare to.

    \param notNull
    The \ilc{nou::NotNull} instance.

    \return
    \ilc{true}, if \p other is smaller than the wrapped pointer of \p notNull, true if not.

    \brief
    Compares two pointers.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    constexpr boolean operator<(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

    /**
    \param other
    The pointer to compare to.

    \param notNull
    The \ilc{nou::NotNull} instance.

    \return
    \ilc{true}, if \p other is larger than the wrapped pointer of \p notNull, true if not.

    \brief
    Compares two pointers.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    constexpr boolean operator>(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

    /**
    \param other
    The pointer to compare to.

    \param notNull
    The \ilc{nou::NotNull} instance.

    \return
    \ilc{true}, if \p other is smaller than or equal to the wrapped pointer of \p notNull, true if not.

    \brief
    Compares two pointers.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    constexpr boolean operator<=(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

    /**
    \param other
    The pointer to compare to.

    \param notNull
    The \ilc{nou::NotNull} instance.

    \return
    \ilc{true}, if \p other is larger than or equal to the wrapped pointer of \p notNull, true if not.

    \brief
    Compares two pointers.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    constexpr boolean operator>=(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

    /**
    \param other
    The pointer to apply the AND operation with.

    \param notNull
    The \ilc{nou::NotNull} instance.

    \return
    \ilc{true} if \p other is not \ilc{nullptr} and \ilc{false} if it is.

    \brief
    Applies a logical AND operation on the two pointers.

    \details
    This function only depends on \p other, since the instance that the operator is called on can never be
    \ilc{nullptr}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    constexpr boolean operator&&(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

    /**
    \param other
    The pointer to apply the OR operation with.

    \param notNull
    The \ilc{nou::NotNull} instance.

    \return
    \ilc{true}.

    \brief
    Applies a logical OR operation on the two pointers.

    \details
    This function always returns \ilc{true}, because, per definition, the right operand is never
    \ilc{nullptr}.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T>
    constexpr boolean operator||(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

    /**
    \param offset
    The value to add to.

    \param notNull
    The pointer to add.

    \return
    An instance that has a wrapped pointer with the value \p offset plus the wrapped pointer of \p notNull.

    \brief
    Increments the wrapped pointer by \p offset and returns the value in a new instance.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T, typename PointerType>
    constexpr NotNull<T> operator+(const PointerType &offset, NotNull<T> &notNull);

    /**
    \param offset
    The value to add to.

    \param notNull
    The pointer to add.

    \return
    An instance that has a wrapped pointer with the value \p offset plus the wrapped pointer of \p notNull.

    \brief
    Increments the wrapped pointer by \p offset and returns the value in a new instance.

    \author  Lukas Reichmann
    \version 1.1.0.0
    \since   1.1.0.0
    */
    template<typename T, typename PointerType>
    constexpr const NotNull<T> operator+(const PointerType &offset, const NotNull<T> &notNull);

    ///\cond

    template<typename T>
    constexpr void NotNull<T>::checkNull() const
    {
        if(!m_ptr)
            ::abort();
    }

    template<typename T>
    constexpr NotNull<T>::NotNull(Type ptr) : m_ptr(ptr)
    {
        checkNull();
    }

    template<typename T>
    inline typename NotNull<T>::Type NotNull<T>::rawPtr()
    {
        return m_ptr;
    }

    template<typename T>
    constexpr typename NotNull<T>::ConstTypeRef NotNull<T>::rawPtr() const
    {
        return m_ptr;
    }

    template<typename T>
    inline typename NotNull<T>::Type &NotNull<T>::operator->()
    {
        return m_ptr;
    }

    template<typename T>
    constexpr typename NotNull<T>::ConstTypeRef NotNull<T>::operator->() const
    {
        return m_ptr;
    }

    template<typename T>
    inline typename NotNull<T>::RawType &NotNull<T>::operator*()
    {
        return *m_ptr;
    }

    template<typename T>
    constexpr const typename NotNull<T>::RawType &NotNull<T>::operator*() const
    {
        return *m_ptr;
    }

    template<typename T>
    inline typename NotNull<T>::RawType &NotNull<T>::operator[](ptrdiffType offset)
    {
        return m_ptr[offset];
    }

    template<typename T>
    constexpr const typename NotNull<T>::RawType &NotNull<T>::operator[](ptrdiffType offset) const
    {
        return m_ptr[offset];
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator==(const NotNull &other) const
    {
        return m_ptr == other.m_ptr;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator==(ConstTypeRef other) const
    {
        return m_ptr == other;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator!=(const NotNull &other) const
    {
        return m_ptr != other.m_ptr;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator!=(ConstTypeRef other) const
    {
        return m_ptr != other;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator<(const NotNull<T> &other) const
    {
        return m_ptr < other.m_ptr;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator<(ConstTypeRef other) const
    {
        return m_ptr < other;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator>(const NotNull<T> &other) const
    {
        return m_ptr > other.m_ptr;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator>(ConstTypeRef other) const
    {
        return m_ptr > other;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator<=(const NotNull<T> &other) const
    {
        return m_ptr <= other.m_ptr;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator<=(ConstTypeRef other) const
    {
        return m_ptr <= other;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator>=(const NotNull<T> &other) const
    {
        return m_ptr >= other.m_ptr;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator>=(ConstTypeRef other) const
    {
        return m_ptr >= other;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator!() const
    {
        return false;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator&&(const NotNull &other) const
    {
        return true;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator&&(ConstTypeRef other) const
    {
        return other;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator||(const NotNull &other) const
    {
        return true;
    }

    template<typename T>
    constexpr boolean NotNull<T>::operator||(ConstTypeRef other) const
    {
        return true;
    }

    template<typename T>
    constexpr NotNull<T>::operator boolean() const
    {
        return true;
    }

    template<typename T>
    inline NotNull<T> &NotNull<T>::operator++()
    {
        ++m_ptr;

        return *this;
    }

    template<typename T>
    inline NotNull<T> &NotNull<T>::operator--()
    {
        --m_ptr;

        return *this;
    }

    template<typename T>
    inline NotNull<T> NotNull<T>::operator++(int)
    {
        NotNull<T> ret = *this;

        ++m_ptr;

        return ret;
    }

    template<typename T>
    inline NotNull<T> NotNull<T>::operator--(int)
    {
        NotNull<T> ret = *this;

        --m_ptr;

        return ret;
    }

    template<typename T>
    template<typename PointerType, typename>
    constexpr NotNull<T> NotNull<T>::operator+(const PointerType &offset) const
    {
        return NotNull<T>(m_ptr + offset);
    }

    template<typename T>
    constexpr ptrdiffType NotNull<T>::operator-(const NotNull &other) const
    {
        return m_ptr - other.m_ptr;
    }

    template<typename T>
    constexpr ptrdiffType NotNull<T>::operator-(ConstTypeRef other) const
    {
        return m_ptr - other;
    }

    template<typename T>
    template<typename PointerType, typename>
    constexpr NotNull<T> NotNull<T>::operator-(const PointerType &offset) const
    {
        return NotNull<T>(m_ptr - offset);
    }

    template<typename T>
    inline NotNull<T> &NotNull<T>::operator=(Type ptr)
    {
        m_ptr = ptr;

        checkNull();

        return *this;
    }

    template<typename T>
    template<typename PointerType>
    inline NotNull<T> &NotNull<T>::operator+=(const PointerType &offset)
    {
        m_ptr += offset;

        checkNull();

        return *this;
    }

    template<typename T>
    template<typename PointerType>
    inline NotNull<T> &NotNull<T>::operator-=(const PointerType &offset)
    {
        m_ptr -= offset;

        checkNull();

        return *this;
    }

    template<typename T>
    constexpr boolean operator==(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
    {
        return other == notNull.rawPtr();
    }

    template<typename T>
    constexpr boolean operator!=(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
    {
        return other != notNull.rawPtr();
    }

    template<typename T>
    constexpr boolean operator<(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
    {
        return other < notNull.rawPtr();
    }

    template<typename T>
    constexpr boolean operator>(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
    {
        return other > notNull.rawPtr();
    }

    template<typename T>
    constexpr boolean operator<=(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
    {
        return other <= notNull.rawPtr();
    }

    template<typename T>
    constexpr boolean operator>=(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
    {
        return other >= notNull.rawPtr();
    }

    template<typename T>
    constexpr boolean operator&&(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
    {
        return other;
    }

    template<typename T>
    constexpr boolean operator||(typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
    {
        return true;
    }

    template<typename T, typename PointerType>
    constexpr NotNull<T> operator+(const PointerType &offset, NotNull<T> &notNull)
    {
        return NotNull<T>(offset + notNull.rawPtr());
    }

    template<typename T, typename PointerType>
    constexpr const NotNull<T> operator+(const PointerType &offset, const NotNull<T> &notNull)
    {
        return NotNull<T>(offset + notNull.rawPtr());
    }

    ///\endcond
} // namespace nou

#endif
