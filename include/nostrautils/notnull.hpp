#ifndef NOU_NOT_NULL_HPP
#define NOU_NOT_NULL_HPP

/**
\file nostrautils/notnull.hpp

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0

\brief
A component that provides a wrapper for (smart-) pointer types that prevents the wrapped pointer to be set to
<code>nullptr</code>.

\details
If a user attempts to assign a <code>nullptr</code> to an instance of <code>nou::NotNull</code>, that 
instance will either trigger a compile error (whenever possible) or a runtime error as soon as the assignment 
is attempted.

Other than that, instances of <code>nou::NotNull</code> will behave like a regular pointer.

<b>Example:</b>
\code{.cpp}
int main()
{
	nou::NotNull<nou::int32*> iPtr1 = nullptr; //results in a compile error

	nou::int32 *ptr = nullptr;
	nou::NotNull<nou::int32*> iPtr2 = ptr; //results in a runtime error
}
\endcode

For a more detailed example, see \link notnull.ex.cpp here\endlink.

<code>nou::NotNull</code> supports <code>constexpr</code> whenever possible. If <code>nullptr</code> is 
assigned to an instance in a case that would usually trigger a runtime error, a compile error is triggered
instead. 

\note
<code>nou::NotNull</code> on its own is not a smart pointer - it does not prevent a user from copying 
pointers nor does it any memory management.
*/

/**
\example notnull.ex.cpp

An example that demonstrates the usage of the notnull component.
*/

#ifndef NOU_TYPES_HPP
#include "nostrautils/types.hpp"
#endif

#include <cstdlib>

namespace nou
{
	namespace internal
	{
		/**
		\tparam T
		The type to convert.

		\return
		<code>const T</endcode>, even if \p T is a pointer type.

		\brief
		A meta function that adds <code>const</code> to a type, even if that type is a pointer.

		\details
		The 'return type' is stored in <code>Type</>.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		template<typename T>
		struct NotNullMakeConstType
		{
			using Type = const T;
		};

		///\cond
		//No need to have this specialization in the documentation
		template<typename T>
		struct NotNullMakeConstType<T*>
		{
			using Type = const T*;
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
		The 'return type' is stored in <code>Type</>.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		template<typename T>
		struct NotNullMakeRefType
		{
			/**
			\brief
			The 'return type' of this meta function.

			\author  Lukas Reichmann
			\version 1.1.0.0
			\since   1.1.0.0
			*/
			using Type = T&;
		};

		///\cond
		//No need to have this specialization in the documentation
		template<typename T>
		struct NotNullMakeRefType<T*>
		{
			using Type = T*;
		};
		///\endcond
	}

	/**
	\tparam T 
	The pointer type that this class wraps around. Not that, if e.g. this class should emulate an 
	<code>int*</code>, this type needs to be <code>int*</code> as well, not just <code>int</code>.

	\brief 
	A class that wraps around a pointer and prevents that pointer from becoming <code>nullptr</code>.
	
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
		The same type as <code>Type</code>, <code>const</code>.

		\details
		<b>Example:</b> 
		If <code>Type</code> is <code>nou::int32*</code>, then <code>ConstType</code> is 
		<code>const nou::int32*</code>.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		using ConstType = typename internal::NotNullMakeConstType<Type>::Type;

		/**
		\brief
		Depending on the <code>ConstType</code>, this is <code>ConstType&</code> or just 
		<code>ConstType</code> (see details).

		\details
		<b>Example:</b>
		If <code>ConstType</code> is a pointer type, then <code>ConstTypeRef</code> is
		<code>ConstTypeRef</code>. If <code>ConstType</code> is any other type, then 
		<code>ConstTypeRef</code> is <code>ConstTypeRef&</code>.

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
		Calls <code>abort()</code> if <code>m_ptr</code> is <code>nullptr</code>.

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
		<b>Example:</b>
		<code>nou::NotNull<nou::int32*>::RawType</code> is <code>nou::int32</code>.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		using RawType = decltype(*m_ptr);

		constexpr NotNull() = default;

		NotNull(nullptrType) = delete;

		/**
		\param ptr
		The pointer that will be wrapped. Must not be <code>nullptr</code>.

		\brief
		Constructs a new instance that wraps the passed pointer.

		\details
		If \p ptr is <code>nullptr</code>, <code>::abort()</code> will be called.

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

		\warning
		This function could be exploited to bypass the <code>nullptr</code> check of this class. This is 
		strongly advised against.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		inline Type& rawPtr();

		/**
		\return
		The wrapped pointer as-is.

		\brief
		Returns the wrapped pointer as-is.

		\details
		This function is supposed to enable compatibility with other functions that required a raw pointer.

		\warning
		This function could be exploited to bypass the <code>nullptr</code> check of this class. This is
		strongly advised against.

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
		inline Type& operator -> ();

		/**
		\return
		A reference to the wrapped pointer.

		\brief
		Calls the arrow operator as if it was called directly on the wrapped pointer.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr ConstTypeRef operator -> () const;

		/**
		\return
		A reference to the object that the wrapped pointer points to.

		\brief
		Calls the indirection (asterisk) operator as if it was called directly on the wrapped pointer.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		inline RawType& operator * ();

		/**
		\return
		A reference to the object that the wrapped pointer points to.

		\brief
		Calls the indirection (asterisk) operator as if it was called directly on the wrapped pointer.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr const RawType& operator * () const;

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
		inline RawType& operator [] (ptrdiffType offset);

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
		constexpr const RawType& operator [] (ptrdiffType offset) const;

		/**
		\param other
		The pointer to compare to.

		\return
		<code>true</code>, if the wrapped pointer of the calling instance and the wrapped pointer of the 
		passed instance are equal, <code>false</code> if not.

		\brief
		Compares two pointers on equality.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator == (const NotNull &other) const;

		/**
		\param other
		The pointer to compare to.

		\return
		<code>true</code>, if the wrapped pointer of the calling instance is equal to the passed pointer, 
		<code>false</code> if not.

		\brief
		Compares two pointers on equality.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator == (ConstTypeRef other) const;

		/**
		\param other
		The pointer to compare to.

		\return
		False, if the wrapped pointer of the calling instance and the wrapped pointer of the passed instance
		are equal, <code>true</code> if not.

		\brief
		Compares two pointers on inequality.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator != (const NotNull &other) const;

		/**
		\param other
		The pointer to compare to.

		\return
		False, if the wrapped pointer of the calling instance is equal to the passed pointer, 
		<code>true</code> if not.

		\brief
		Compares two pointers on inequality.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator != (ConstTypeRef other) const;

		/**
		\param other
		The pointer to compare to.

		\return
		<code>true</code>, if the wrapped pointer of the calling instance is smaller than the wrapped pointer 
		of the passed instance, <code>true</code> if not.

		\brief
		Compares two pointers.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator < (const NotNull &other) const;

		/**
		\param other
		The pointer to compare to.

		\return
		<code>true</code>, if the wrapped pointer of the calling instance is smaller than the passed pointer, 
		<code>true</code> if not.

		\brief
		Compares two pointers.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator < (ConstTypeRef other) const;

		/**
		\param other
		The pointer to compare to.

		\return
		<code>true</code>, if the wrapped pointer of the calling instance is larger than the wrapped pointer 
		of the passed instance, <code>true</code> if not.

		\brief
		Compares two pointers.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator > (const NotNull &other) const;

		/**
		\param other
		The pointer to compare to.

		\return
		<code>true</code>, if the wrapped pointer of the calling instance is larger than the passed pointer, 
		<code>true</code> if not.

		\brief
		Compares two pointers.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator > (ConstTypeRef other) const;

		/**
		\param other
		The pointer to compare to.

		\return
		<code>true</code>, if the wrapped pointer of the calling instance is smaller than or equal to the 
		wrapped pointer of the passed instance, <code>true</code> if not.

		\brief
		Compares two pointers.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator <= (const NotNull &other) const;

		/**
		\param other
		The pointer to compare to.

		\return
		<code>true</code>, if the wrapped pointer of the calling instance is smaller than or equal to the 
		passed pointer, <code>true</code> if not.

		\brief
		Compares two pointers.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator <= (ConstTypeRef other) const;

		/**
		\param other
		The pointer to compare to.

		\return
		<code>true</code>, if the wrapped pointer of the calling instance is larger than or equal to the 
		wrapped pointer of the passed instance, <code>true</code> if not.

		\brief
		Compares two pointers.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator >= (const NotNull &other) const;

		/**
		\param other
		The pointer to compare to.

		\return
		<code>true</code>, if the wrapped pointer of the calling instance is larger than or equal to the 
		passed pointer, <code>true</code> if not.

		\brief
		Compares two pointers.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator >= (ConstTypeRef other) const;

		/**
		\return
		<code>false</code>.

		\brief
		Returns whether the wrapped pointer is <code>nullptr</code>, which is obviously always 
		<code>false</code>.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator ! () const;

		/**
		\param other
		The pointer to apply the AND operation with.

		\return
		<code>true</code>.

		\brief
		Applies a logical AND operation on the two pointers.

		\details
		This function always returns <code>true</code>, because, per definition, both operands are never
		<code>nullptr</code>.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator && (const NotNull &other) const;

		/**
		\param other
		The pointer to apply the AND operation with.

		\return
		<code>true</code> if \p other is not <code>nullptr</code> and <code>false</code> if it is.

		\brief
		Applies a logical AND operation on the two pointers.

		\details
		This function only depends on \p other, since the instance that the operator is called on can never
		be <code>nullptr</code>.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator && (ConstTypeRef other) const;

		/**
		\param other
		The pointer to apply the OR operation with.

		\return
		<code>true</code>.

		\brief
		Applies a logical OR operation on the two pointers.

		\details
		This function always returns <code>true</code>, because, per definition, both operands are never
		<code>nullptr</code>.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator || (const NotNull &other) const;

		/**
		\param other
		The pointer to apply the OR operation with.

		\return
		<code>true</code>.

		\brief
		Applies a logical OR operation on the two pointers.

		\details
		This function always returns <code>true</code>, because, per definition, the left operand is never
		<code>nullptr</code>.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr boolean operator || (ConstTypeRef other) const;

		/**
		\return
		<code>true</code>.

		\brief
		Returns whether the wrapped pointer is not <code>nullptr</code>, which is obviously always true.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		constexpr operator boolean () const;

		/**
		\return
		A reference to the instance that the operator was called on.

		\brief 
		Increments the wrapped pointer by one.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		inline NotNull& operator ++ ();

		/**
		\return
		A reference to the instance that the operator was called on.

		\brief
		Decrements the wrapped pointer by one.

		\warning
		For performance reasons, this operator does not check whether the wrapped pointer will be
		<code>nullptr</code> after the decrementation.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		inline NotNull& operator -- ();

		/**
		\return
		A copy of the instance from before the incrementation took place.

		\brief
		Increments the wrapped pointer by one.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		inline NotNull operator ++ (int);

		/**
		\return
		A copy of the instance from before the incrementation took place.

		\brief
		Decrements the wrapped pointer by one.

		\warning
		For performance reasons, this operator does not check whether the wrapped pointer will be 
		<code>nullptr</code> after the decrementation.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		inline NotNull operator -- (int);

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
		constexpr NotNull operator + (ptrdiffType offset) const;

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
		constexpr ptrdiffType operator - (const NotNull &other) const;

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
		constexpr ptrdiffType operator - (ConstTypeRef other) const;

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
		constexpr NotNull operator - (ptrdiffType offset) const;

		NotNull& operator = (nullptrType) = delete;

		/**
		\param ptr
		The pointer that will be wrapped. Must not be <code>nullptr</code>.

		\return
		A reference to the instance that the operator was called on.

		\brief
		Sets the wrapped pointer to the passed one.

		\details
		If \p ptr is <code>nullptr</code>, <code>::abort()</code> will be called.

		\author  Lukas Reichmann
		\version 1.1.0.0
		\since   1.1.0.0
		*/
		inline NotNull& operator = (Type ptr);
	};

	/**
	\param other
	The pointer to compare to.

	\param notNull
	The nou::NotNull instance.

	\return
	<code>true</code>, if the wrapped pointer of \p notNull is equal to \p other, <code>false</code> if not.

	\brief
	Compares two pointers on equality.

	\author  Lukas Reichmann
	\version 1.1.0.0
	\since   1.1.0.0
	*/
	template<typename T>
	constexpr boolean operator == (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

	/**
	\param other
	The pointer to compare to.

	\param notNull
	The nou::NotNull instance.

	\return
	<code>false</code>, if the wrapped pointer of \p notNull is equal to \p other, <code>true</code> if not.

	\brief
	Compares two pointers on inequality.

	\author  Lukas Reichmann
	\version 1.1.0.0
	\since   1.1.0.0
	*/
	template<typename T>
	constexpr boolean operator != (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

	/**
	\param other
	The pointer to compare to.

	\param notNull
	The nou::NotNull instance.

	\return
	<code>true</code>, if \p other is smaller than the wrapped pointer of \p notNull, true if not.

	\brief
	Compares two pointers.

	\author  Lukas Reichmann
	\version 1.1.0.0
	\since   1.1.0.0
	*/
	template<typename T>
	constexpr boolean operator < (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

	/**
	\param other
	The pointer to compare to.

	\param notNull
	The nou::NotNull instance.

	\return
	<code>true</code>, if \p other is larger than the wrapped pointer of \p notNull, true if not.

	\brief
	Compares two pointers.

	\author  Lukas Reichmann
	\version 1.1.0.0
	\since   1.1.0.0
	*/
	template<typename T>
	constexpr boolean operator > (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

	/**
	\param other
	The pointer to compare to.

	\param notNull
	The nou::NotNull instance.

	\return
	<code>true</code>, if \p other is smaller than or equal to the wrapped pointer of \p notNull, true if 
	not.

	\brief
	Compares two pointers.

	\author  Lukas Reichmann
	\version 1.1.0.0
	\since   1.1.0.0
	*/
	template<typename T>
	constexpr boolean operator <= (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

	/**
	\param other
	The pointer to compare to.

	\param notNull
	The nou::NotNull instance.

	\return
	<code>true</code>, if \p other is larger than or equal to the wrapped pointer of \p notNull, true if not.

	\brief
	Compares two pointers.

	\author  Lukas Reichmann
	\version 1.1.0.0
	\since   1.1.0.0
	*/
	template<typename T>
	constexpr boolean operator >= (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

	/**
	\param other
	The pointer to apply the AND operation with.

	\param notNull
	The nou::NotNull instance.

	\return
	<code>true</code> if \p other is not <code>nullptr</code> and <code>false</code> if it is.

	\brief
	Applies a logical AND operation on the two pointers.

	\details
	This function only depends on \p other, since the instance that the operator is called on can never
	be <code>nullptr</code>.

	\author  Lukas Reichmann
	\version 1.1.0.0
	\since   1.1.0.0
	*/
	template<typename T>
	constexpr boolean operator && (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

	/**
	\param other
	The pointer to apply the OR operation with.

	\param notNull
	The nou::NotNull instance.

	\return
	<code>true</code>.

	\brief
	Applies a logical OR operation on the two pointers.

	\details
	This function always returns <code>true</code>, because, per definition, the right operand is never
	<code>nullptr</code>.

	\author  Lukas Reichmann
	\version 1.1.0.0
	\since   1.1.0.0
	*/
	template<typename T>
	constexpr boolean operator || (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull);

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
	template<typename T>
	constexpr NotNull<T> operator + (ptrdiffType offset, NotNull<T> &notNull);

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
	template<typename T>
	constexpr const NotNull<T> operator + (ptrdiffType offset, const NotNull<T> &notNull);



	template<typename T>
	constexpr void NotNull<T>::checkNull() const
	{
		if(!m_ptr)
			::abort();
	}

	template<typename T>
	constexpr NotNull<T>::NotNull(Type ptr) : 
		m_ptr(ptr)
	{
		checkNull();
	}

	template<typename T>
	inline typename NotNull<T>::Type& NotNull<T>::rawPtr()
	{
		return m_ptr;
	}

	template<typename T>
	constexpr typename NotNull<T>::ConstTypeRef NotNull<T>::rawPtr() const
	{
		return m_ptr;
	}

	template<typename T>
	inline typename NotNull<T>::Type& NotNull<T>::operator -> ()
	{
		return m_ptr;
	}

	template<typename T>
	constexpr typename NotNull<T>::ConstTypeRef NotNull<T>::operator -> () const
	{
		return m_ptr;
	}

	template<typename T>
	inline typename NotNull<T>::RawType& NotNull<T>::operator * ()
	{
		return *m_ptr;
	}

	template<typename T>
	constexpr const typename NotNull<T>::RawType& NotNull<T>::operator * () const
	{
		return *m_ptr;
	}

	template<typename T>
	inline typename NotNull<T>::RawType& NotNull<T>::operator [] (ptrdiffType offset)
	{
		return m_ptr[offset];
	}

	template<typename T>
	constexpr const typename NotNull<T>::RawType& NotNull<T>::operator [] (ptrdiffType offset) const
	{
		return m_ptr[offset];
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator == (const NotNull &other) const
	{
		return m_ptr == other.m_ptr;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator == (ConstTypeRef other) const
	{
		return m_ptr == other;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator != (const NotNull &other) const
	{
		return m_ptr != other.m_ptr;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator != (ConstTypeRef other) const
	{
		return m_ptr != other;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator < (const NotNull<T> &other) const
	{
		return m_ptr < other.m_ptr;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator < (ConstTypeRef other) const
	{
		return m_ptr < other;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator > (const NotNull<T> &other) const
	{
		return m_ptr > other.m_ptr;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator > (ConstTypeRef other) const
	{
		return m_ptr > other;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator <= (const NotNull<T> &other) const
	{
		return m_ptr <= other.m_ptr;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator <= (ConstTypeRef other) const
	{
		return m_ptr <= other;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator >= (const NotNull<T> &other) const
	{
		return m_ptr >= other.m_ptr;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator >= (ConstTypeRef other) const
	{
		return m_ptr >= other;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator ! () const
	{
		return false;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator && (const NotNull &other) const
	{
		return true;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator && (ConstTypeRef other) const
	{
		return other;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator || (const NotNull &other) const
	{
		return true;
	}

	template<typename T>
	constexpr boolean NotNull<T>::operator || (ConstTypeRef other) const
	{
		return true;
	}

	template<typename T>
	constexpr NotNull<T>::operator boolean () const
	{
		return true;
	}

	template<typename T>
	inline NotNull<T>& NotNull<T>::operator ++ ()
	{
		++m_ptr;

		return *this;
	}

	template<typename T>
	inline NotNull<T>& NotNull<T>::operator -- ()
	{
		--m_ptr;

		return *this;
	}

	template<typename T>
	inline NotNull<T> NotNull<T>::operator ++ (int)
	{
		NotNull<T> ret = *this;

		++m_ptr;

		return ret;
	}

	template<typename T>
	inline NotNull<T> NotNull<T>::operator -- (int)
	{
		NotNull<T> ret = *this;

		--m_ptr;

		return ret;
	}

	template<typename T>
	constexpr NotNull<T> NotNull<T>::operator + (ptrdiffType offset) const
	{
		return NotNull<T>(m_ptr + offset);
	}

	template<typename T>
	constexpr ptrdiffType NotNull<T>::operator - (const NotNull &other) const
	{
		return m_ptr - other.m_ptr;
	}

	template<typename T>
	constexpr ptrdiffType NotNull<T>::operator - (ConstTypeRef other) const
	{
		return m_ptr - other;
	}

	template<typename T>
	constexpr NotNull<T> NotNull<T>::operator - (ptrdiffType offset) const
	{
		return NotNull<T>(m_ptr - offset);
	}

	template<typename T>
	inline NotNull<T>& NotNull<T>::operator = (Type ptr)
	{
		m_ptr = ptr;

		checkNull();

		return *this;
	}



	template<typename T>
	constexpr boolean operator == (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
	{
		return other == notNull.rawPtr();
	}

	template<typename T>
	constexpr boolean operator != (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
	{
		return other != notNull.rawPtr();
	}

	template<typename T>
	constexpr boolean operator < (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
	{
		return other < notNull.rawPtr();
	}

	template<typename T>
	constexpr boolean operator > (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
	{
		return other > notNull.rawPtr();
	}

	template<typename T>
	constexpr boolean operator <= (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
	{
		return other <= notNull.rawPtr();
	}

	template<typename T>
	constexpr boolean operator >= (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
	{
		return other >= notNull.rawPtr();
	}

	template<typename T>
	constexpr boolean operator && (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
	{
		return other;
	}

	template<typename T>
	constexpr boolean operator || (typename NotNull<T>::ConstTypeRef other, const NotNull<T> &notNull)
	{
		return true;
	}

	template<typename T>
	constexpr NotNull<T> operator + (ptrdiffType offset, NotNull<T> &notNull) 
	{
		return NotNull<T>(offset + notNull.rawPtr());
	}

	template<typename T>
	constexpr const NotNull<T> operator + (ptrdiffType offset, const NotNull<T> &notNull)
	{
		return NotNull<T>(offset + notNull.rawPtr());
	}
}

#endif
