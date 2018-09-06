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
	/**
	\tparam T 
	The pointer type that this class wraps around. Not that, if e.g. this class should emulate an 
	<code>int*</code>, this type needs to be <code>int*</code> as well, not just <code>int</code>.

	\brief 
	A class that wraps around a pointer and prevents that pointer from becoming <code>nullptr</code>.
	
	\details
	See notnull.hpp for a full description.

	\see notnull.hpp
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
		constexpr const Type& rawPtr() const;

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
		constexpr const Type& operator -> () const;

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

		NotNull& operator = (nullptrType) = delete;

		/**
		\param ptr
		The pointer that will be wrapped. Must not be <code>nullptr</code>.

		\return
		A reference to instance that the operator was called on.

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
	constexpr const typename NotNull<T>::Type& NotNull<T>::rawPtr() const
	{
		return m_ptr;
	}

	template<typename T>
	inline typename NotNull<T>::Type& NotNull<T>::operator -> ()
	{
		return m_ptr;
	}

	template<typename T>
	constexpr const typename NotNull<T>::Type& NotNull<T>::operator -> () const
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
	inline NotNull<T>& NotNull<T>::operator = (Type ptr)
	{
		m_ptr = ptr;

		checkNull();

		return *this;
	}
}

#endif
