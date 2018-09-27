/**
File:          meta.wt.cpp
Author:        Lukas Reichmann
Creation Date: 27.09.2018 (dd.mm.yyyy)

A test for meta.
*/

#ifndef NOU_TYPES_HPP
#    include "nostrautils/types.hpp"
#endif

#ifndef NOU_META_HPP
#    include "nostrautils/meta.hpp"
#endif

#include <cstdlib>
#include <cstring>
#include <iostream>

#define NOU_TEST_IS_TRUE(...)                                \
    if(!(__VA_ARGS__))                                       \
    {                                                        \
        std::cout << "Failed in line: " << __LINE__ << "\n"; \
        exit(__LINE__);                                      \
    }

#define NOU_TEST_IS_EQUAL(a, b) NOU_TEST_IS_TRUE(((a) == (b)))

class Test
{};

enum class TestEnumClass
{
};

enum TestEnum
{
};

union TestUnion
{};

void testfunc()
{}

int main()
{
    using namespace nou;

    // AreSame is must work in order for all of the remaining tests to work
    NOU_TEST_IS_TRUE(AreSame<int32, int32>::value);
    NOU_TEST_IS_TRUE(AreSame<float32, float32>::value);
    NOU_TEST_IS_TRUE(AreSame<Test, Test>::value);
    NOU_TEST_IS_TRUE(!AreSame<int32, float32>::value);
    NOU_TEST_IS_TRUE(!AreSame<int32, Test>::value);

    // Identity
    NOU_TEST_IS_TRUE(AreSame<typename Identity<int32>::Type, int32>::value);

    // Constant(s)
    NOU_TEST_IS_TRUE(AreSame<typename Constant<int32, 5>::Type, int32>::value);
    NOU_TEST_IS_TRUE(Constant<int32, 5>::value == 5);

    NOU_TEST_IS_TRUE(BooleanConstant<true>::value == true);
    NOU_TEST_IS_TRUE(BooleanConstant<false>::value == false);

    NOU_TEST_IS_TRUE(TrueConstant::value == true);
    NOU_TEST_IS_TRUE(FalseConstant::value == false);

    // If
    NOU_TEST_IS_TRUE(AreSame<typename If<true, int32, float32>::Type, int32>::value);
    NOU_TEST_IS_TRUE(AreSame<typename If<false, int32, float32>::Type, float32>::value);

    NOU_TEST_IS_TRUE(AreSame<IfType<true, int32, float32>, int32>::value);
    NOU_TEST_IS_TRUE(AreSame<IfType<false, int32, float32>, float32>::value);

    // Get
    NOU_TEST_IS_TRUE(AreSame<typename Get<0, int32, float32, Test>::Type, int32>::value);
    NOU_TEST_IS_TRUE(AreSame<typename Get<1, int32, float32, Test>::Type, float32>::value);
    NOU_TEST_IS_TRUE(AreSame<typename Get<2, int32, float32, Test>::Type, Test>::value);

    NOU_TEST_IS_TRUE(AreSame<typename Get<0, int32, float32, Test>::Type, int32>::value);
    NOU_TEST_IS_TRUE(AreSame<typename Get<1, int32, float32, Test>::Type, float32>::value);
    NOU_TEST_IS_TRUE(AreSame<typename Get<2, int32, float32, Test>::Type, Test>::value);

    NOU_TEST_IS_TRUE(AreSame<GetType<0, int32, float32, Test>, int32>::value);
    NOU_TEST_IS_TRUE(AreSame<GetType<1, int32, float32, Test>, float32>::value);
    NOU_TEST_IS_TRUE(AreSame<GetType<2, int32, float32, Test>, Test>::value);

    NOU_TEST_IS_TRUE(AreSame<GetType<0, int32, float32, Test>, int32>::value);
    NOU_TEST_IS_TRUE(AreSame<GetType<1, int32, float32, Test>, float32>::value);
    NOU_TEST_IS_TRUE(AreSame<GetType<2, int32, float32, Test>, Test>::value);

    // IsArray
    NOU_TEST_IS_TRUE(IsArray<int32[]>::value);
    NOU_TEST_IS_TRUE(!IsArray<int32>::value);

    // IsVoid
    NOU_TEST_IS_TRUE(IsVoid<void>::value);
    NOU_TEST_IS_TRUE(!IsVoid<int32>::value);

    // IsNullptrType
    NOU_TEST_IS_TRUE(IsNullptrType<decltype(nullptr)>::value);
    NOU_TEST_IS_TRUE(IsNullptrType<nullptrType>::value);
    NOU_TEST_IS_TRUE(!IsNullptrType<int32>::value);

    // IsIntegral
    NOU_TEST_IS_TRUE(IsIntegral<int32>::value);
    NOU_TEST_IS_TRUE(IsIntegral<char8>::value);
    NOU_TEST_IS_TRUE(!IsIntegral<float32>::value);
    NOU_TEST_IS_TRUE(!IsIntegral<Test>::value);

    // IsFloatingPoint
    NOU_TEST_IS_TRUE(IsFloatingPoint<float32>::value);
    NOU_TEST_IS_TRUE(IsFloatingPoint<float64>::value);
    NOU_TEST_IS_TRUE(!IsFloatingPoint<int32>::value);
    NOU_TEST_IS_TRUE(!IsFloatingPoint<Test>::value);

    // IsEnum
    NOU_TEST_IS_TRUE(IsEnum<TestEnumClass>::value);
    NOU_TEST_IS_TRUE(IsEnum<TestEnum>::value);
    NOU_TEST_IS_TRUE(!IsEnum<int32>::value);
    NOU_TEST_IS_TRUE(!IsEnum<Test>::value);

    // IsUnion
    NOU_TEST_IS_TRUE(IsUnion<TestUnion>::value);
    NOU_TEST_IS_TRUE(!IsUnion<int32>::value);
    NOU_TEST_IS_TRUE(!IsUnion<Test>::value);

    // IsClass
    NOU_TEST_IS_TRUE(IsClass<Test>::value);
    NOU_TEST_IS_TRUE(!IsClass<int32>::value);
    NOU_TEST_IS_TRUE(!IsClass<TestUnion>::value);

    // IsFunction
    NOU_TEST_IS_TRUE(IsFunction<decltype(testfunc)>::value);
    NOU_TEST_IS_TRUE(!IsFunction<int32>::value);
    NOU_TEST_IS_TRUE(!IsFunction<TestUnion>::value);

    // IsPointer
    NOU_TEST_IS_TRUE(IsPointer<int32 *>::value);
    NOU_TEST_IS_TRUE(IsPointer<const int32 *>::value);
    NOU_TEST_IS_TRUE(!IsPointer<int32>::value);

    // IsLValueReference
    NOU_TEST_IS_TRUE(IsLValueReference<int32 &>::value);
    NOU_TEST_IS_TRUE(IsLValueReference<const int32 &>::value);
    NOU_TEST_IS_TRUE(!IsLValueReference<int32>::value);

    // IsRValueReference
    NOU_TEST_IS_TRUE(IsRValueReference<int32 &&>::value);
    NOU_TEST_IS_TRUE(IsRValueReference<const int32 &&>::value);
    NOU_TEST_IS_TRUE(!IsRValueReference<int32>::value);

    // IsMemberObjectPointer
    NOU_TEST_IS_TRUE(IsMemberObjectPointer<int32(Test::*)>::value);
    NOU_TEST_IS_TRUE(!IsMemberObjectPointer<int32 (Test::*)(int32)>::value);
    NOU_TEST_IS_TRUE(!IsMemberObjectPointer<int32>::value);

    // IsMemberFunctionPointer
    NOU_TEST_IS_TRUE(IsMemberFunctionPointer<int32 (Test::*)(int32)>::value);
    NOU_TEST_IS_TRUE(!IsMemberFunctionPointer<int32(Test::*)>::value);
    NOU_TEST_IS_TRUE(!IsMemberFunctionPointer<int32>::value);

    // IsFundamental
    NOU_TEST_IS_TRUE(IsFundamental<int32>::value);
    NOU_TEST_IS_TRUE(IsFundamental<float32>::value);
    NOU_TEST_IS_TRUE(IsFundamental<boolean>::value);
    NOU_TEST_IS_TRUE(!IsFundamental<Test>::value);
    NOU_TEST_IS_TRUE(!IsFundamental<TestEnum>::value);

    // IsArithmetic
    NOU_TEST_IS_TRUE(IsArithmetic<int32>::value);
    NOU_TEST_IS_TRUE(IsArithmetic<float32>::value);
    NOU_TEST_IS_TRUE(IsArithmetic<boolean>::value);
    NOU_TEST_IS_TRUE(!IsArithmetic<Test>::value);
    NOU_TEST_IS_TRUE(!IsArithmetic<TestEnum>::value);

    // IsScalar
    NOU_TEST_IS_TRUE(IsScalar<int32>::value);
    NOU_TEST_IS_TRUE(IsScalar<float32>::value);
    NOU_TEST_IS_TRUE(IsScalar<boolean>::value);
    NOU_TEST_IS_TRUE(IsScalar<int32 *>::value);
    NOU_TEST_IS_TRUE(IsScalar<int32(Test::*)>::value);
    NOU_TEST_IS_TRUE(IsScalar<int32 (Test::*)(int32)>::value);
    NOU_TEST_IS_TRUE(IsScalar<TestEnumClass>::value);
    NOU_TEST_IS_TRUE(IsScalar<TestEnum>::value);
    NOU_TEST_IS_TRUE(IsScalar<nullptrType>::value);
    NOU_TEST_IS_TRUE(!IsScalar<Test>::value);
    NOU_TEST_IS_TRUE(!IsScalar<TestUnion>::value);

    // IsObject
    NOU_TEST_IS_TRUE(IsObject<int32>::value);
    NOU_TEST_IS_TRUE(IsObject<float32>::value);
    NOU_TEST_IS_TRUE(!IsObject<int32(int32)>::value);
    NOU_TEST_IS_TRUE(!IsObject<int32 &>::value);
    NOU_TEST_IS_TRUE(!IsObject<void>::value);

    // IsCompound
    NOU_TEST_IS_TRUE(IsCompound<int32[]>::value);
    NOU_TEST_IS_TRUE(IsCompound<int32(int32)>::value);
    NOU_TEST_IS_TRUE(IsCompound<int32 *>::value);
    NOU_TEST_IS_TRUE(IsCompound<int32 (*)(int32)>::value);
    NOU_TEST_IS_TRUE(IsCompound<int32(Test::*)>::value);
    NOU_TEST_IS_TRUE(IsCompound<int32 (Test::*)(int32)>::value);
    NOU_TEST_IS_TRUE(IsCompound<int32 &>::value);
    NOU_TEST_IS_TRUE(IsCompound<Test>::value);
    NOU_TEST_IS_TRUE(IsCompound<TestUnion>::value);
    NOU_TEST_IS_TRUE(IsCompound<TestEnumClass>::value);
    NOU_TEST_IS_TRUE(IsCompound<TestEnum>::value);
    NOU_TEST_IS_TRUE(!IsCompound<nullptrType>::value);
    NOU_TEST_IS_TRUE(!IsCompound<void>::value);
    NOU_TEST_IS_TRUE(!IsCompound<int32>::value);

    // IsMemberPointer
    NOU_TEST_IS_TRUE(IsMemberPointer<int32(Test::*)>::value);
    NOU_TEST_IS_TRUE(IsMemberPointer<int32 (Test::*)(int32)>::value);
    NOU_TEST_IS_TRUE(!IsMemberPointer<int32>::value);
    NOU_TEST_IS_TRUE(!IsMemberPointer<int32 *>::value);

    // IsConst
    NOU_TEST_IS_TRUE(IsConst<const int32>::value);
    NOU_TEST_IS_TRUE(!IsConst<int32>::value);

    // IsVolatile
    NOU_TEST_IS_TRUE(IsVolatile<volatile int32>::value);
    NOU_TEST_IS_TRUE(!IsVolatile<int32>::value);

    // TODO: finish tests
}
