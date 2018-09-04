#ifndef NOU_TYPES_HPP
#define NOU_TYPES_HPP

/**
\file nostrautils/types.hpp

\author  Lukas Reichmann
\version 1.1.0.0
\since   1.1.0.0

\brief
A component that provides the fundamental types that are used throughout the entire library.

\details
Since the C++ standard lacks clearly defined value boundaries for primitive datatypes, but it is desirable to
have such fixed boundaries, this component provides such types with a fixed width along all platforms. These
types are equivalent to their respective type in the STL (<code>nou::int32</code> is the same type as <code>
std::int32_t</code>, or just <code>int32_t</code> from the C standard library for that matter).

<b>Example:</b>
\code{.cpp}
int main()
{
	nou::int32 integer = 5; //nou::int32 is same as std::int32_t
	nou::float32 floatingPoint = 3.0f; //nou::float32 has no equivalent in the STL
}
\endcode
*/

#include <cstdint>
#include <type_traits>

namespace nou
{
	/**
	\brief 
	A datatype that is exactly one byte large.

	\details
	This type should be used when allocating memory on a byte-level to enhance readability.

	<b>Example:</b>
	\code{.cpp}
	nou::byte memory[sizeof(nou::int32)]; //allocate enough memory to hold a nou::int32 as a byte array
	\endcode
	*/
	using byte = unsigned char;

	/**
	\brief 
	A datatype to store boolean values.

	\details
	This types behaves the same way that the build-in type <code>bool</code> does.
	*/
	using boolean = bool;

	/**
	\brief 
	A datatype that can store 8-bit characters.
	*/
	using char8 = char;

	/**
	\brief 
	A datatype that can store 16-bit characters.
	*/
	using char16 = char16_t;

	/**
	\brief 
	A datatype that can store 32-bit characters.
	*/
	using char32 = char32_t;

	/**
	\brief 
	A datatype that can store 8-bit signed integers.
	*/
	using int8 = std::int8_t;

	/**
	\brief 
	A datatype that can store 16-bit signed integers.
	*/
	using int16 = std::int16_t;

	/**
	\brief 
	A datatype that can store 32-bit signed integers.
	*/
	using int32 = std::int32_t;

	/**
	\brief 
	A datatype that can store 64-bit signed integers.
	*/
	using int64 = std::int64_t;

	/**
	\brief 
	A datatype that can store 8-bit unsigned integers.
	*/
	using uint8 = std::uint8_t;

	/**
	\brief 
	A datatype that can store 16-bit unsigned integers.
	*/
	using uint16 = std::uint16_t;

	/**
	\brief 
	A datatype that can store 32-bit unsigned integers.
	*/
	using uint32 = std::uint32_t;

	/**
	\brief 
	A datatype that can store 64-bit unsigned integers.
	*/
	using uint64 = std::uint64_t;

	namespace internal
	{
		/**
		\tparam SIZE The size of the desired floating point type in bytes.

		\brief 
		A metafunction that returns a floating point type with the passed size \p SIZE.

		\details
		The returned type is in the member type alias <code>type</code>.

		If a size is passed that is smaller than any of the available floating point types, the smallest of 
		the available types will be returned.

		If a size is passed that is larger than any of the available floating point types, the larges of the
		available types will be returned.

		\see nou::internal::ChooseFloat_t
		*/
		template<int32 SIZE>
		struct ChooseFloat :
			std::conditional<sizeof(float) >= SIZE,
				float,
				typename std::conditional_t<sizeof(double) >= SIZE,
					double,
					long double
				>
			> {};

		/**
		\brief 
		Defined as the resulting type of nou::internal::ChooseFloat.

		\see nou::internal::ChooseFloat
		*/
		template<int32 SIZE>
		using ChooseFloat_t = typename ChooseFloat<SIZE>::type;
	}

	/**
	\brief
	A floating point datatype that is 32 bit large (if possible).

	\note
	If the compiler does not provide a 32 bit floating point type, this type will either be the largest 
	possible type, in the case that there are only smaller type, or, in the case that there are only larger 
	types it will be the smallest available type.
	*/
	using float32 = internal::ChooseFloat_t<4>;

	/**
	\brief
	A floating point datatype that is 64 bit large (if possible).

	\note
	If the compiler does not provide a 64 bit floating point type, this type will either be the largest
	possible type, in the case that there are only smaller type, or, in the case that there are only larger
	types it will be the smallest available type.
	*/
	using float64 = internal::ChooseFloat_t<8>;
}

#endif