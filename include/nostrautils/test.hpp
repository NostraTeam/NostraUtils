
//To be removed later

#ifndef NOU_TEST_HPP
#define NOU_TEST_HPP

#ifdef NOU_EXPORT_SYMBOLS
#define NOU_EXPORT __declspec(dllexport)
#else
#define NOU_EXPORT __declspec(dllimport)
#endif

#undef NOU_EXPORT

#define NOU_EXPORT

namespace nou
{
	NOU_EXPORT int func(int i);
}

#endif