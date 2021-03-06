//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <math.h>

// This file was copied from libc++'s test suite, then modified to test CUDA.
// For the most part, this consists of adding __device__ attributes and
// deleting long double.

// This test requires C++11 (it's mostly decltype checks).
#if __cplusplus >= 201103L

#include <math.h>
#include <type_traits>
#include <cassert>
#include <stdio.h>

// See PR21083
// Ambiguous is a user-defined type that defines its own overloads of cmath
// functions. When the std overloads are candidates too (by using or adl),
// they should not interfere.
struct Ambiguous : std::true_type { // ADL
    __device__ operator float () { return 0.f; }
    __device__ operator double () { return 0.; }
};
__device__ Ambiguous abs(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous acos(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous asin(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous atan(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous atan2(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous ceil(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous cos(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous cosh(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous exp(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous fabs(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous floor(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous fmod(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous frexp(Ambiguous, int*){ return Ambiguous(); }
__device__ Ambiguous ldexp(Ambiguous, int){ return Ambiguous(); }
__device__ Ambiguous log(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous log10(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous modf(Ambiguous, Ambiguous*){ return Ambiguous(); }
__device__ Ambiguous pow(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous sin(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous sinh(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous sqrt(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous tan(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous tanh(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous signbit(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous fpclassify(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous isfinite(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous isnormal(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous isgreater(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous isgreaterequal(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous isless(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous islessequal(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous islessgreater(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous isunordered(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous acosh(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous asinh(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous atanh(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous cbrt(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous copysign(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous erf(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous erfc(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous exp2(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous expm1(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous fdim(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous fma(Ambiguous, Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous fmax(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous fmin(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous hypot(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous ilogb(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous lgamma(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous llrint(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous llround(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous log1p(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous log2(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous logb(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous lrint(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous lround(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous nearbyint(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous nextafter(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous remainder(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous remquo(Ambiguous, Ambiguous, int*){ return Ambiguous(); }
__device__ Ambiguous rint(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous round(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous scalbln(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous scalbn(Ambiguous, Ambiguous){ return Ambiguous(); }
__device__ Ambiguous tgamma(Ambiguous){ return Ambiguous(); }
__device__ Ambiguous trunc(Ambiguous){ return Ambiguous(); }


// helper function to prevent compiler constant-folding test inputs.

template <typename T>
__device__ T V(T input) {
  volatile T tmp = input;
  return tmp;
}

__device__ void test_abs()
{
    static_assert((std::is_same<decltype(abs((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(abs((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(abs(Ambiguous())), Ambiguous>::value), "");
    assert(abs(V(-1)) == 1);
    assert(abs(V(-1.)) == 1);
    assert(abs(V(-1.f)) == 1);
}

__device__ void test_acos()
{
    static_assert((std::is_same<decltype(acos((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(acos((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(acos((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(acos((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(acos((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(acos((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(acos((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(acos((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(acos((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(acos((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(acosf(0)), float>::value), "");
    static_assert((std::is_same<decltype(acos(Ambiguous())), Ambiguous>::value), "");
    assert(acos(V(1)) == 0);
    assert(acos(V(1.)) == 0);
    assert(acos(V(1.f)) == 0);
}

__device__ void test_asin()
{
    static_assert((std::is_same<decltype(asin((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(asin((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(asin((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(asin((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(asin((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(asin((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(asin((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(asin((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(asin((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(asin((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(asinf(0)), float>::value), "");
    static_assert((std::is_same<decltype(asin(Ambiguous())), Ambiguous>::value), "");
    assert(asin(V(0)) == 0);
    assert(asin(V(0.)) == 0);
    assert(asin(V(0.f)) == 0);
}

__device__ void test_atan()
{
    static_assert((std::is_same<decltype(atan((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(atan((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(atanf(0)), float>::value), "");
    static_assert((std::is_same<decltype(atan(Ambiguous())), Ambiguous>::value), "");
    assert(atan(V(0)) == 0);
    assert(atan(V(0.)) == 0);
    assert(atan(V(0.f)) == 0);
}

__device__ void test_atan2()
{
    static_assert((std::is_same<decltype(atan2((float)0, (float)0)), float>::value), "");
    static_assert((std::is_same<decltype(atan2((bool)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan2((unsigned short)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan2((float)0, (unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan2((double)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan2((int)0, (long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan2((int)0, (unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan2((double)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan2((float)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan2f(0,0)), float>::value), "");
    static_assert((std::is_same<decltype(atan2((int)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(atan2(Ambiguous(), Ambiguous())), Ambiguous>::value), "");
    assert(atan2(V(0), 1) == 0);
    assert(atan2(V(0), 1.) == 0);
    assert(atan2(V(0), 1.f) == 0);

    assert(atan2(V(0.), 1) == 0);
    assert(atan2(V(0.), 1.) == 0);
    assert(atan2(V(0.), 1.f) == 0);

    assert(atan2(V(0.f), 1) == 0);
    assert(atan2(V(0.f), 1.) == 0);
    assert(atan2(V(0.f), 1.f) == 0);
}

__device__ void test_ceil()
{
    static_assert((std::is_same<decltype(ceil((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(ceil((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(ceil((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(ceil((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(ceil((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(ceil((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(ceil((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(ceil((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(ceil((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(ceil((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(ceilf(0)), float>::value), "");
    static_assert((std::is_same<decltype(ceil(Ambiguous())), Ambiguous>::value), "");
    assert(ceil(V(0)) == 0);
    assert(ceil(V(0.)) == 0);
    assert(ceil(V(0.f)) == 0);
}

__device__ void test_cos()
{
    static_assert((std::is_same<decltype(cos((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(cos((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(cos((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(cos((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(cos((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(cos((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(cos((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(cos((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(cos((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(cos((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(cosf(0)), float>::value), "");
    static_assert((std::is_same<decltype(cos(Ambiguous())), Ambiguous>::value), "");
    assert(cos(V(0)) == 1);
    assert(cos(V(0.)) == 1);
    assert(cos(V(0.f)) == 1);
}

__device__ void test_cosh()
{
    static_assert((std::is_same<decltype(cosh((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(cosh((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(cosh((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(cosh((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(cosh((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(cosh((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(cosh((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(cosh((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(cosh((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(cosh((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(coshf(0)), float>::value), "");
    static_assert((std::is_same<decltype(cosh(Ambiguous())), Ambiguous>::value), "");
    assert(cosh(V(0)) == 1);
    assert(cosh(V(0.)) == 1);
    assert(cosh(V(0.f)) == 1);
}

__device__ void test_exp()
{
    static_assert((std::is_same<decltype(exp((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(exp((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(expf(0)), float>::value), "");
    static_assert((std::is_same<decltype(exp(Ambiguous())), Ambiguous>::value), "");
    assert(exp(V(0)) == 1);
    assert(exp(V(0.)) == 1);
    assert(exp(V(0.f)) == 1);
}

__device__ void test_fabs()
{
    static_assert((std::is_same<decltype(fabs((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(fabs((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(fabs((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(fabs((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(fabs((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(fabs((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fabs((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fabs((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fabs((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fabs((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fabsf(0.0f)), float>::value), "");
    static_assert((std::is_same<decltype(fabs(Ambiguous())), Ambiguous>::value), "");
    assert(fabs(V(-1)) == 1);
    assert(fabs(V(-1.)) == 1);
    assert(fabs(V(-1.f)) == 1);
}

__device__ void test_floor()
{
    static_assert((std::is_same<decltype(floor((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(floor((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(floor((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(floor((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(floor((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(floor((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(floor((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(floor((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(floor((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(floor((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(floorf(0)), float>::value), "");
    static_assert((std::is_same<decltype(floor(Ambiguous())), Ambiguous>::value), "");
    assert(floor(V(1)) == 1);
    assert(floor(V(1.)) == 1);
    assert(floor(V(1.f)) == 1);
}

__device__ void test_fmod()
{
    static_assert((std::is_same<decltype(fmod((float)0, (float)0)), float>::value), "");
    static_assert((std::is_same<decltype(fmod((bool)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmod((unsigned short)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmod((float)0, (unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmod((double)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmod((int)0, (long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmod((int)0, (unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmod((double)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmod((float)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmodf(0,0)), float>::value), "");
    static_assert((std::is_same<decltype(fmod((int)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmod(Ambiguous(), Ambiguous())), Ambiguous>::value), "");

    assert(fmod(V(1.5), 1) == .5);
    assert(fmod(V(1.5), 1.) == .5);
    assert(fmod(V(1.5), 1.f) == .5);

    assert(fmod(V(1.5f), 1) == .5);
    assert(fmod(V(1.5f), 1.) == .5);
    assert(fmod(V(1.5f), 1.f) == .5);

    assert(fmod(V(2), 1) == 0);
    assert(fmod(V(2), 1.) == 0);
    assert(fmod(V(2), 1.f) == 0);
}

__device__ void test_frexp()
{
    int ip;
    static_assert((std::is_same<decltype(frexp((float)0, &ip)), float>::value), "");
    static_assert((std::is_same<decltype(frexp((bool)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(frexp((unsigned short)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(frexp((int)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(frexp((unsigned int)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(frexp((long)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(frexp((unsigned long)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(frexp((long long)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(frexp((unsigned long long)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(frexp((double)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(frexpf(0, &ip)), float>::value), "");
    static_assert((std::is_same<decltype(frexp(Ambiguous(), &ip)), Ambiguous>::value), "");
    assert(frexp(V(0), &ip) == 0);
    assert(frexp(V(0.), &ip) == 0);
    assert(frexp(V(0.f), &ip) == 0);
}

__device__ void test_ldexp()
{
    int ip = 1;
    static_assert((std::is_same<decltype(ldexp((float)0, ip)), float>::value), "");
    static_assert((std::is_same<decltype(ldexp((bool)0, ip)), double>::value), "");
    static_assert((std::is_same<decltype(ldexp((unsigned short)0, ip)), double>::value), "");
    static_assert((std::is_same<decltype(ldexp((int)0, ip)), double>::value), "");
    static_assert((std::is_same<decltype(ldexp((unsigned int)0, ip)), double>::value), "");
    static_assert((std::is_same<decltype(ldexp((long)0, ip)), double>::value), "");
    static_assert((std::is_same<decltype(ldexp((unsigned long)0, ip)), double>::value), "");
    static_assert((std::is_same<decltype(ldexp((long long)0, ip)), double>::value), "");
    static_assert((std::is_same<decltype(ldexp((unsigned long long)0, ip)), double>::value), "");
    static_assert((std::is_same<decltype(ldexp((double)0, ip)), double>::value), "");
    static_assert((std::is_same<decltype(ldexpf(0, ip)), float>::value), "");
    static_assert((std::is_same<decltype(ldexp(Ambiguous(), ip)), Ambiguous>::value), "");
    assert(ldexp(V(1), ip) == 2);
    assert(ldexp(V(1.), ip) == 2);
    assert(ldexp(V(1.f), ip) == 2);
}

__device__ void test_log()
{
    static_assert((std::is_same<decltype(log((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(log((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(log((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(log((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(log((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(log((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(logf(0)), float>::value), "");
    static_assert((std::is_same<decltype(log(Ambiguous())), Ambiguous>::value), "");
    assert(log(V(1)) == 0);
    assert(log(V(1.)) == 0);
    assert(log(V(1.f)) == 0);
}

__device__ void test_log10()
{
    static_assert((std::is_same<decltype(log10((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(log10((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(log10((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(log10((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(log10((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(log10((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log10((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log10((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log10((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log10((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(log10f(0)), float>::value), "");
    static_assert((std::is_same<decltype(log10(Ambiguous())), Ambiguous>::value), "");
    assert(log10(V(1)) == 0);
    assert(log10(V(1.)) == 0);
    assert(log10(V(1.f)) == 0);
}

__device__ void test_modf()
{
    static_assert((std::is_same<decltype(modf((float)0, (float*)0)), float>::value), "");
    static_assert((std::is_same<decltype(modf((double)0, (double*)0)), double>::value), "");
    static_assert((std::is_same<decltype(modff(0, (float*)0)), float>::value), "");
    static_assert((std::is_same<decltype(modf(Ambiguous(), (Ambiguous*)0)), Ambiguous>::value), "");
    double i;
    assert(modf(V(1), &i) == 0);
    assert(modf(V(1.), &i) == 0);
    assert(modf(V(1.f), &i) == 0);
}

__device__ void test_pow()
{
    static_assert((std::is_same<decltype(pow((float)0, (float)0)), float>::value), "");
    static_assert((std::is_same<decltype(pow((bool)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(pow((unsigned short)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(pow((float)0, (unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(pow((double)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(pow((int)0, (long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(pow((int)0, (unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(pow((double)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(pow((float)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(powf(0,0)), float>::value), "");
    static_assert((std::is_same<decltype(pow((int)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(pow(Ambiguous(), Ambiguous())), Ambiguous>::value), "");
    assert(pow(V(1), 1) == 1);
    assert(pow(V(1.), 1) == 1);
    assert(pow(V(1.f), 1) == 1);

    assert(pow(V(1), 1.) == 1);
    assert(pow(V(1.), 1.) == 1);
    assert(pow(V(1.f), 1.) == 1);

    assert(pow(V(1), 1.f) == 1);
    assert(pow(V(1.), 1.f) == 1);
    assert(pow(V(1.f), 1.f) == 1);
}

__device__ void test_sin()
{
    static_assert((std::is_same<decltype(sin((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(sin((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(sin((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(sin((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(sin((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(sin((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(sin((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(sin((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(sin((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(sin((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(sinf(0)), float>::value), "");
    static_assert((std::is_same<decltype(sin(Ambiguous())), Ambiguous>::value), "");
    assert(sin(0) == 0);
    assert(sin(0.) == 0);
    assert(sin(0.f) == 0);
}

__device__ void test_sinh()
{
    static_assert((std::is_same<decltype(sinh((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(sinh((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(sinh((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(sinh((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(sinh((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(sinh((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(sinh((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(sinh((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(sinh((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(sinh((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(sinhf(0)), float>::value), "");
    static_assert((std::is_same<decltype(sinh(Ambiguous())), Ambiguous>::value), "");
    assert(sinh(V(0)) == 0);
    assert(sinh(V(0.)) == 0);
    assert(sinh(V(0.f)) == 0);
}

__device__ void test_sqrt()
{
    static_assert((std::is_same<decltype(sqrt((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(sqrt((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(sqrt((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(sqrt((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(sqrt((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(sqrt((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(sqrt((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(sqrt((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(sqrt((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(sqrt((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(sqrtf(0)), float>::value), "");
    static_assert((std::is_same<decltype(sqrt(Ambiguous())), Ambiguous>::value), "");
    assert(sqrt(V(4)) == 2);
    assert(sqrt(V(4.)) == 2);
    assert(sqrt(V(4.f)) == 2);
}

__device__ void test_tan()
{
    static_assert((std::is_same<decltype(tan((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(tan((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(tan((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(tan((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(tan((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(tan((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(tan((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(tan((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(tan((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(tan((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(tanf(0)), float>::value), "");
    static_assert((std::is_same<decltype(tan(Ambiguous())), Ambiguous>::value), "");
    assert(tan(V(0)) == 0);
    assert(tan(V(0.)) == 0);
    assert(tan(V(0.f)) == 0);
}

__device__ void test_tanh()
{
    static_assert((std::is_same<decltype(tanh((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(tanh((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(tanh((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(tanh((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(tanh((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(tanh((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(tanh((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(tanh((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(tanh((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(tanh((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(tanhf(0)), float>::value), "");
    static_assert((std::is_same<decltype(tanh(Ambiguous())), Ambiguous>::value), "");
    assert(tanh(V(0)) == 0);
    assert(tanh(V(0.)) == 0);
    assert(tanh(V(0.f)) == 0);
}

__device__ void test_signbit()
{
#ifdef signbit
#error signbit defined
#endif
    static_assert((std::is_same<decltype(signbit((float)0)), bool>::value), "");
    static_assert((std::is_same<decltype(signbit((double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(signbit(0)), bool>::value), "");
    static_assert((std::is_same<decltype(signbit(Ambiguous())), Ambiguous>::value), "");
    assert(signbit(V(-1)) == true);
    assert(signbit(V(-1.)) == true);
    assert(signbit(V(-1.f)) == true);
}

__device__ void test_fpclassify()
{
#ifdef fpclassify
#error fpclassify defined
#endif
    static_assert((std::is_same<decltype(fpclassify((float)0)), int>::value), "");
    static_assert((std::is_same<decltype(fpclassify((double)0)), int>::value), "");
    static_assert((std::is_same<decltype(fpclassify(0)), int>::value), "");
    static_assert((std::is_same<decltype(fpclassify(Ambiguous())), Ambiguous>::value), "");
    assert(fpclassify(V(-1)) == FP_NORMAL);
    assert(fpclassify(V(-1.)) == FP_NORMAL);
    assert(fpclassify(V(-1.f)) == FP_NORMAL);
}

__device__ void test_isfinite()
{
#ifdef isfinite
#error isfinite defined
#endif
    static_assert((std::is_same<decltype(isfinite((float)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isfinite((double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isfinite(0)), bool>::value), "");
    static_assert((std::is_same<decltype(isfinite(Ambiguous())), Ambiguous>::value), "");
    assert(isfinite(V(-1)) == true);
    assert(isfinite(V(-1.)) == true);
    assert(isfinite(V(-1.f)) == true);
}

__device__ void test_isnormal()
{
#ifdef isnormal
#error isnormal defined
#endif
    static_assert((std::is_same<decltype(isnormal((float)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isnormal((double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isnormal(0)), bool>::value), "");
    static_assert((std::is_same<decltype(isnormal(Ambiguous())), Ambiguous>::value), "");
    assert(std::isnormal(V(-1)) == true);
    assert(std::isnormal(V(-1.)) == true);
    assert(std::isnormal(V(-1.f)) == true);
}

__device__ void test_isgreater()
{
#ifdef isgreater
#error isgreater defined
#endif
    static_assert((std::is_same<decltype(isgreater((float)0, (float)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isgreater((float)0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isgreater((double)0, (float)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isgreater((double)0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isgreater(0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isgreater(Ambiguous(), Ambiguous())), Ambiguous>::value), "");
    assert(std::isgreater(V(-1), 0) == false);
    assert(std::isgreater(V(-1), 0.) == false);
    assert(std::isgreater(V(-1), 0.f) == false);

    assert(std::isgreater(V(-1.), 0) == false);
    assert(std::isgreater(V(-1.), 0.) == false);
    assert(std::isgreater(V(-1.), 0.f) == false);

    assert(std::isgreater(V(-1.f), 0) == false);
    assert(std::isgreater(V(-1.f), 0.) == false);
    assert(std::isgreater(V(-1.f), 0.f) == false);
}

__device__ void test_isgreaterequal()
{
#ifdef isgreaterequal
#error isgreaterequal defined
#endif
    static_assert((std::is_same<decltype(isgreaterequal((float)0, (float)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isgreaterequal((float)0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isgreaterequal((double)0, (float)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isgreaterequal((double)0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isgreaterequal(0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isgreaterequal(Ambiguous(), Ambiguous())), Ambiguous>::value), "");
    assert(std::isgreaterequal(V(-1), 0) == false);
    assert(std::isgreaterequal(V(-1), 0.) == false);
    assert(std::isgreaterequal(V(-1), 0.f) == false);

    assert(std::isgreaterequal(V(-1.), 0) == false);
    assert(std::isgreaterequal(V(-1.), 0.) == false);
    assert(std::isgreaterequal(V(-1.), 0.f) == false);

    assert(std::isgreaterequal(V(-1.f), 0) == false);
    assert(std::isgreaterequal(V(-1.f), 0.) == false);
    assert(std::isgreaterequal(V(-1.f), 0.f) == false);
}

__device__ void test_isinf()
{
#ifdef isinf
#error isinf defined
#endif
    static_assert((std::is_same<decltype(isinf((float)0)), bool>::value), "");

    typedef decltype(isinf((double)0)) DoubleRetType;
#ifndef __linux__
    static_assert((std::is_same<DoubleRetType, bool>::value), "");
#else
    // GLIBC < 2.26 defines 'isinf(double)' with a return type of 'int' in
    // all C++ dialects. The test should tolerate this.
    // See: https://sourceware.org/bugzilla/show_bug.cgi?id=19439
    static_assert((std::is_same<DoubleRetType, bool>::value
                || std::is_same<DoubleRetType, int>::value), "");
#endif

    static_assert((std::is_same<decltype(isinf(0)), bool>::value), "");
    assert(std::isinf(V(-1)) == false);
    assert(std::isinf(V(-1.)) == false);
    assert(std::isinf(V(-1.f)) == false);
}

__device__ void test_isless()
{
#ifdef isless
#error isless defined
#endif
    static_assert((std::is_same<decltype(isless((float)0, (float)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isless((float)0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isless((double)0, (float)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isless((double)0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isless(0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isless(Ambiguous(), Ambiguous())), Ambiguous>::value), "");
    assert(std::isless(V(-1), 0) == true);
    assert(std::isless(V(-1), 0.) == true);
    assert(std::isless(V(-1), 0.f) == true);

    assert(std::isless(V(-1.), 0) == true);
    assert(std::isless(V(-1.), 0.) == true);
    assert(std::isless(V(-1.), 0.f) == true);

    assert(std::isless(V(-1.f), 0) == true);
    assert(std::isless(V(-1.f), 0.) == true);
    assert(std::isless(V(-1.f), 0.f) == true);
}

__device__ void test_islessequal()
{
#ifdef islessequal
#error islessequal defined
#endif
    static_assert((std::is_same<decltype(islessequal((float)0, (float)0)), bool>::value), "");
    static_assert((std::is_same<decltype(islessequal((float)0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(islessequal((double)0, (float)0)), bool>::value), "");
    static_assert((std::is_same<decltype(islessequal((double)0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(islessequal(0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(islessequal(Ambiguous(), Ambiguous())), Ambiguous>::value), "");
    assert(std::islessequal(V(-1), 0) == true);
    assert(std::islessequal(V(-1), 0.) == true);
    assert(std::islessequal(V(-1), 0.f) == true);

    assert(std::islessequal(V(-1.), 0) == true);
    assert(std::islessequal(V(-1.), 0.) == true);
    assert(std::islessequal(V(-1.), 0.f) == true);

    assert(std::islessequal(V(-1.f), 0) == true);
    assert(std::islessequal(V(-1.f), 0.) == true);
    assert(std::islessequal(V(-1.f), 0.f) == true);
}

__device__ void test_islessgreater()
{
#ifdef islessgreater
#error islessgreater defined
#endif
    static_assert((std::is_same<decltype(islessgreater((float)0, (float)0)), bool>::value), "");
    static_assert((std::is_same<decltype(islessgreater((float)0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(islessgreater((double)0, (float)0)), bool>::value), "");
    static_assert((std::is_same<decltype(islessgreater((double)0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(islessgreater(0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(islessgreater(Ambiguous(), Ambiguous())), Ambiguous>::value), "");
    assert(std::islessgreater(V(-1), 0) == true);
    assert(std::islessgreater(V(-1), 0.) == true);
    assert(std::islessgreater(V(-1), 0.f) == true);

    assert(std::islessgreater(V(-1.), 0) == true);
    assert(std::islessgreater(V(-1.), 0.) == true);
    assert(std::islessgreater(V(-1.), 0.f) == true);

    assert(std::islessgreater(V(-1.f), 0) == true);
    assert(std::islessgreater(V(-1.f), 0.) == true);
    assert(std::islessgreater(V(-1.f), 0.f) == true);
}

__device__ void test_isnan()
{
#ifdef isnan
#error isnan defined
#endif
    static_assert((std::is_same<decltype(isnan((float)0)), bool>::value), "");

    typedef decltype(isnan((double)0)) DoubleRetType;
#ifndef __linux__
    static_assert((std::is_same<DoubleRetType, bool>::value), "");
#else
    // GLIBC < 2.26 defines 'isnan(double)' with a return type of 'int' in
    // all C++ dialects. The test should tolerate this.
    // See: https://sourceware.org/bugzilla/show_bug.cgi?id=19439
    static_assert((std::is_same<DoubleRetType, bool>::value
                || std::is_same<DoubleRetType, int>::value), "");
#endif

    static_assert((std::is_same<decltype(isnan(0)), bool>::value), "");
    assert(std::isnan(V(-1)) == false);
    assert(std::isnan(V(-1.)) == false);
    assert(std::isnan(V(-1.f)) == false);
}

__device__ void test_isunordered()
{
#ifdef isunordered
#error isunordered defined
#endif
    static_assert((std::is_same<decltype(isunordered((float)0, (float)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isunordered((float)0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isunordered((double)0, (float)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isunordered((double)0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isunordered(0, (double)0)), bool>::value), "");
    static_assert((std::is_same<decltype(isunordered(Ambiguous(), Ambiguous())), Ambiguous>::value), "");
    assert(std::isunordered(V(-1), 0) == false);
    assert(std::isunordered(V(-1), 0.) == false);
    assert(std::isunordered(V(-1), 0.f) == false);

    assert(std::isunordered(V(-1.), 0) == false);
    assert(std::isunordered(V(-1.), 0.) == false);
    assert(std::isunordered(V(-1.), 0.f) == false);

    assert(std::isunordered(V(-1.f), 0) == false);
    assert(std::isunordered(V(-1.f), 0.) == false);
    assert(std::isunordered(V(-1.f), 0.f) == false);
}

__device__ void test_acosh()
{
    static_assert((std::is_same<decltype(acosh((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(acosh((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(acosh((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(acosh((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(acosh((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(acosh((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(acosh((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(acosh((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(acosh((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(acosh((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(acoshf(0)), float>::value), "");
    static_assert((std::is_same<decltype(acosh(Ambiguous())), Ambiguous>::value), "");
    assert(std::acosh(V(1)) == 0);
    assert(std::acosh(V(1.)) == 0);
    assert(std::acosh(V(1.f)) == 0);
}

__device__ void test_asinh()
{
    static_assert((std::is_same<decltype(asinh((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(asinh((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(asinh((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(asinh((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(asinh((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(asinh((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(asinh((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(asinh((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(asinh((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(asinh((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(asinhf(0)), float>::value), "");
    static_assert((std::is_same<decltype(asinh(Ambiguous())), Ambiguous>::value), "");
    assert(asinh(V(0)) == 0);
    assert(asinh(V(0.)) == 0);
    assert(asinh(V(0.f)) == 0);
}

__device__ void test_atanh()
{
    static_assert((std::is_same<decltype(atanh((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(atanh((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(atanh((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(atanh((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(atanh((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(atanh((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(atanh((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(atanh((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(atanh((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(atanh((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(atanhf(0)), float>::value), "");
    static_assert((std::is_same<decltype(atanh(Ambiguous())), Ambiguous>::value), "");
    assert(atanh(V(0)) == 0);
    assert(atanh(V(0.)) == 0);
    assert(atanh(V(0.f)) == 0);
}

__device__ void test_cbrt()
{
    static_assert((std::is_same<decltype(cbrt((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(cbrt((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(cbrt((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(cbrt((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(cbrt((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(cbrt((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(cbrt((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(cbrt((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(cbrt((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(cbrt((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(cbrtf(0)), float>::value), "");
    static_assert((std::is_same<decltype(cbrt(Ambiguous())), Ambiguous>::value), "");
    assert(cbrt(V(1)) == 1);
    assert(cbrt(V(1.)) == 1);
    assert(cbrt(V(1.f)) == 1);
}

__device__ void test_copysign()
{
    static_assert((std::is_same<decltype(copysign((float)0, (float)0)), float>::value), "");
    static_assert((std::is_same<decltype(copysign((bool)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(copysign((unsigned short)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(copysign((float)0, (unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(copysign((double)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(copysign((int)0, (long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(copysign((int)0, (unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(copysign((double)0, (double)0)), double>::value), "");

    // CUDA's copysign(float, double) currently returns a float, in violation
    // of the spec.  We can't easily change this, so accept either one.
    static_assert(
        (std::is_same<decltype(copysign((float)0, (double)0)), double>::value ||
         std::is_same<decltype(copysign((float)0, (double)0)), float>::value),
        "");

    static_assert((std::is_same<decltype(copysignf(0,0)), float>::value), "");
    static_assert((std::is_same<decltype(copysign((int)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(copysign(Ambiguous(), Ambiguous())), Ambiguous>::value), "");

    assert(std::copysign(V(1), 1) == 1);
    assert(std::copysign(V(1.), 1) == 1);
    assert(std::copysign(V(1.f), 1) == 1);

    assert(std::copysign(V(1), 1.) == 1);
    assert(std::copysign(V(1.), 1.) == 1);
    assert(std::copysign(V(1.f), 1.) == 1);

    assert(std::copysign(V(1), 1.f) == 1);
    assert(std::copysign(V(1.), 1.f) == 1);
    assert(std::copysign(V(1.f), 1.f) == 1);
}

__device__ void test_erf()
{
    static_assert((std::is_same<decltype(erf((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(erf((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(erf((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(erf((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(erf((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(erf((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(erf((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(erf((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(erf((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(erf((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(erff(0)), float>::value), "");
    static_assert((std::is_same<decltype(erf(Ambiguous())), Ambiguous>::value), "");
    assert(erf(V(0)) == 0);
    assert(erf(V(0.)) == 0);
    assert(erf(V(0.f)) == 0);
}

__device__ void test_erfc()
{
    static_assert((std::is_same<decltype(erfc((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(erfc((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(erfc((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(erfc((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(erfc((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(erfc((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(erfc((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(erfc((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(erfc((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(erfc((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(erfcf(0)), float>::value), "");
    static_assert((std::is_same<decltype(erfc(Ambiguous())), Ambiguous>::value), "");
    assert(erfc(V(0)) == 1);
    assert(erfc(V(0.)) == 1);
    assert(erfc(V(0.f)) == 1);
}

__device__ void test_exp2()
{
    static_assert((std::is_same<decltype(exp2((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(exp2((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp2((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp2((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp2((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp2((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp2((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp2((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp2((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp2((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(exp2f(0)), float>::value), "");
    static_assert((std::is_same<decltype(exp2(Ambiguous())), Ambiguous>::value), "");
    assert(exp2(V(1)) == 2);
    assert(exp2(V(1.)) == 2);
    assert(exp2(V(1.f)) == 2);
}

__device__ void test_expm1()
{
    static_assert((std::is_same<decltype(expm1((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(expm1((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(expm1((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(expm1((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(expm1((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(expm1((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(expm1((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(expm1((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(expm1((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(expm1((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(expm1f(0)), float>::value), "");
    static_assert((std::is_same<decltype(expm1(Ambiguous())), Ambiguous>::value), "");
    assert(expm1(V(0)) == 0);
    assert(expm1(V(0.)) == 0);
    assert(expm1(V(0.f)) == 0);
}

__device__ void test_fdim()
{
    static_assert((std::is_same<decltype(fdim((float)0, (float)0)), float>::value), "");
    static_assert((std::is_same<decltype(fdim((bool)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(fdim((unsigned short)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fdim((float)0, (unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(fdim((double)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fdim((int)0, (long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fdim((int)0, (unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fdim((double)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fdim((float)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fdimf(0,0)), float>::value), "");
    static_assert((std::is_same<decltype(fdim((int)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(fdim(Ambiguous(), Ambiguous())), Ambiguous>::value), "");

    assert(std::fdim(V(1), 0) == 1);
    assert(std::fdim(V(1.), 0) == 1);
    assert(std::fdim(V(1.f), 0) == 1);

    assert(std::fdim(V(1), 0.) == 1);
    assert(std::fdim(V(1.), 0.) == 1);
    assert(std::fdim(V(1.f), 0.) == 1);

    assert(std::fdim(V(1), 0.f) == 1);
    assert(std::fdim(V(1.), 0.f) == 1);
    assert(std::fdim(V(1.f), 0.f) == 1);
}

__device__ void test_fma()
{
    static_assert((std::is_same<decltype(fma((bool)0, (float)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(fma((char)0, (float)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(fma((unsigned)0, (float)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(fma((float)0, (int)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(fma((float)0, (long)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(fma((float)0, (float)0, (unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fma((float)0, (float)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fma((float)0, (float)0, (float)0)), float>::value), "");

    static_assert((std::is_same<decltype(fma((bool)0, (double)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fma((char)0, (double)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fma((unsigned)0, (double)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fma((double)0, (int)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fma((double)0, (long)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fma((double)0, (double)0, (unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fma((double)0, (double)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(fma((double)0, (double)0,  (double)0)), double>::value), "");


    static_assert((std::is_same<decltype(fmaf(0,0,0)), float>::value), "");
    static_assert((std::is_same<decltype(fma(Ambiguous(), Ambiguous(), Ambiguous())), Ambiguous>::value), "");

    assert(std::fma(V(1), 1, 1) == 2);
    assert(std::fma(V(1.), 1, 1) == 2);
    assert(std::fma(V(1.f), 1, 1) == 2);
    assert(std::fma(V(1), 1., 1) == 2);
    assert(std::fma(V(1.), 1., 1) == 2);
    assert(std::fma(V(1.f), 1., 1) == 2);
    assert(std::fma(V(1), 1.f, 1) == 2);
    assert(std::fma(V(1.), 1.f, 1) == 2);
    assert(std::fma(V(1.f), 1.f, 1) == 2);

    assert(std::fma(V(1), 1, 1.) == 2);
    assert(std::fma(V(1.), 1, 1.) == 2);
    assert(std::fma(V(1.f), 1, 1.) == 2);
    assert(std::fma(V(1), 1., 1.) == 2);
    assert(std::fma(V(1.), 1., 1.) == 2);
    assert(std::fma(V(1.f), 1., 1.) == 2);
    assert(std::fma(V(1), 1.f, 1.) == 2);
    assert(std::fma(V(1.), 1.f, 1.) == 2);
    assert(std::fma(V(1.f), 1.f, 1.) == 2);

    assert(std::fma(V(1), 1, 1.f) == 2);
    assert(std::fma(V(1.), 1, 1.f) == 2);
    assert(std::fma(V(1.f), 1, 1.f) == 2);
    assert(std::fma(V(1), 1., 1.f) == 2);
    assert(std::fma(V(1.), 1., 1.f) == 2);
    assert(std::fma(V(1.f), 1., 1.f) == 2);
    assert(std::fma(V(1), 1.f, 1.f) == 2);
    assert(std::fma(V(1.), 1.f, 1.f) == 2);
    assert(std::fma(V(1.f), 1.f, 1.f) == 2);
}

__device__ void test_fmax()
{
    static_assert((std::is_same<decltype(fmax((float)0, (float)0)), float>::value), "");
    static_assert((std::is_same<decltype(fmax((bool)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmax((unsigned short)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmax((float)0, (unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmax((double)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmax((int)0, (long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmax((int)0, (unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmax((double)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmax((float)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmaxf(0,0)), float>::value), "");
    static_assert((std::is_same<decltype(fmax((int)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmax(Ambiguous(), Ambiguous())), Ambiguous>::value), "");

    assert(std::fmax(V(1), 0) == 1);
    assert(std::fmax(V(1.), 0) == 1);
    assert(std::fmax(V(1.f), 0) == 1);

    assert(std::fmax(V(1), 0.) == 1);
    assert(std::fmax(V(1.), 0.) == 1);
    assert(std::fmax(V(1.f), 0.) == 1);

    assert(std::fmax(V(1), 0.f) == 1);
    assert(std::fmax(V(1.), 0.f) == 1);
    assert(std::fmax(V(1.f), 0.f) == 1);
}

__device__ void test_fmin()
{
    static_assert((std::is_same<decltype(fmin((float)0, (float)0)), float>::value), "");
    static_assert((std::is_same<decltype(fmin((bool)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmin((unsigned short)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmin((float)0, (unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmin((double)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmin((int)0, (long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmin((int)0, (unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmin((double)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmin((float)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(fminf(0,0)), float>::value), "");
    static_assert((std::is_same<decltype(fmin((int)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(fmin(Ambiguous(), Ambiguous())), Ambiguous>::value), "");

    assert(std::fmin(V(1), 0) == 0);
    assert(std::fmin(V(1.), 0) == 0);
    assert(std::fmin(V(1.f), 0) == 0);

    assert(std::fmin(V(1), 0.) == 0);
    assert(std::fmin(V(1.), 0.) == 0);
    assert(std::fmin(V(1.f), 0.) == 0);

    assert(std::fmin(V(1), 0.f) == 0);
    assert(std::fmin(V(1.), 0.f) == 0);
    assert(std::fmin(V(1.f), 0.f) == 0);
}

__device__ void test_hypot()
{
    static_assert((std::is_same<decltype(hypot((float)0, (float)0)), float>::value), "");
    static_assert((std::is_same<decltype(hypot((bool)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(hypot((unsigned short)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(hypot((float)0, (unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(hypot((double)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(hypot((int)0, (long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(hypot((int)0, (unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(hypot((double)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(hypot((float)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(hypotf(0,0)), float>::value), "");
    static_assert((std::is_same<decltype(hypot((int)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(hypot(Ambiguous(), Ambiguous())), Ambiguous>::value), "");

    assert(std::hypot(V(3), 4) == 5);
    assert(std::hypot(V(3), 4.) == 5);
    assert(std::hypot(V(3), 4.f) == 5);

    assert(std::hypot(V(3.), 4) == 5);
    assert(std::hypot(V(3.), 4.) == 5);
    assert(std::hypot(V(3.), 4.f) == 5);

    assert(std::hypot(V(3.f), 4) == 5);
    assert(std::hypot(V(3.f), 4.) == 5);
    assert(std::hypot(V(3.f), 4.f) == 5);
}

__device__ void test_ilogb()
{
    static_assert((std::is_same<decltype(ilogb((float)0)), int>::value), "");
    static_assert((std::is_same<decltype(ilogb((bool)0)), int>::value), "");
    static_assert((std::is_same<decltype(ilogb((unsigned short)0)), int>::value), "");
    static_assert((std::is_same<decltype(ilogb((int)0)), int>::value), "");
    static_assert((std::is_same<decltype(ilogb((unsigned int)0)), int>::value), "");
    static_assert((std::is_same<decltype(ilogb((long)0)), int>::value), "");
    static_assert((std::is_same<decltype(ilogb((unsigned long)0)), int>::value), "");
    static_assert((std::is_same<decltype(ilogb((long long)0)), int>::value), "");
    static_assert((std::is_same<decltype(ilogb((unsigned long long)0)), int>::value), "");
    static_assert((std::is_same<decltype(ilogb((double)0)), int>::value), "");
    static_assert((std::is_same<decltype(ilogbf(0)), int>::value), "");
    static_assert((std::is_same<decltype(ilogb(Ambiguous())), Ambiguous>::value), "");
    assert(ilogb(V(1)) == 0);
    assert(ilogb(V(1.)) == 0);
    assert(ilogb(V(1.f)) == 0);
}

__device__ void test_lgamma()
{
    static_assert((std::is_same<decltype(lgamma((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(lgamma((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(lgamma((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(lgamma((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(lgamma((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(lgamma((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(lgamma((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(lgamma((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(lgamma((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(lgamma((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(lgammaf(0)), float>::value), "");
    static_assert((std::is_same<decltype(lgamma(Ambiguous())), Ambiguous>::value), "");
    assert(lgamma(V(1)) == 0);
    assert(lgamma(V(1.)) == 0);
    assert(lgamma(V(1.f)) == 0);
}

__device__ void test_llrint()
{
    static_assert((std::is_same<decltype(llrint((float)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llrint((bool)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llrint((unsigned short)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llrint((int)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llrint((unsigned int)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llrint((long)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llrint((unsigned long)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llrint((long long)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llrint((unsigned long long)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llrint((double)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llrintf(0)), long long>::value), "");
    static_assert((std::is_same<decltype(llrint(Ambiguous())), Ambiguous>::value), "");
    assert(llrint(V(1)) == 1LL);
    assert(llrint(V(1.)) == 1LL);
#if CUDA_VERSION > 7050
    assert(llrint(V(1.f)) == 1LL);
#endif
}

__device__ void test_llround()
{
    static_assert((std::is_same<decltype(llround((float)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llround((bool)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llround((unsigned short)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llround((int)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llround((unsigned int)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llround((long)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llround((unsigned long)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llround((long long)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llround((unsigned long long)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llround((double)0)), long long>::value), "");
    static_assert((std::is_same<decltype(llroundf(0)), long long>::value), "");
    static_assert((std::is_same<decltype(llround(Ambiguous())), Ambiguous>::value), "");
    assert(llround(V(1)) == 1LL);
    assert(llround(V(1.)) == 1LL);
    assert(llround(V(1.f)) == 1LL);
}

__device__ void test_log1p()
{
    static_assert((std::is_same<decltype(log1p((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(log1p((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(log1p((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(log1p((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(log1p((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(log1p((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log1p((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log1p((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log1p((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log1p((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(log1pf(0)), float>::value), "");
    static_assert((std::is_same<decltype(log1p(Ambiguous())), Ambiguous>::value), "");
    assert(log1p(V(0)) == 0);
    assert(log1p(V(0.)) == 0);
    assert(log1p(V(0.f)) == 0);
}

__device__ void test_log2()
{
    static_assert((std::is_same<decltype(log2((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(log2((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(log2((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(log2((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(log2((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(log2((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log2((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log2((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log2((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(log2((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(log2f(0)), float>::value), "");
    static_assert((std::is_same<decltype(log2(Ambiguous())), Ambiguous>::value), "");
    assert(log2(V(1)) == 0);
    assert(log2(V(1.)) == 0);
    assert(log2(V(1.f)) == 0);
}

__device__ void test_logb()
{
    static_assert((std::is_same<decltype(logb((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(logb((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(logb((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(logb((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(logb((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(logb((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(logb((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(logb((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(logb((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(logb((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(logbf(0)), float>::value), "");
    static_assert((std::is_same<decltype(logb(Ambiguous())), Ambiguous>::value), "");
    assert(logb(V(1)) == 0);
    assert(logb(V(1.)) == 0);
    assert(logb(V(1.f)) == 0);
}

__device__ void test_lrint()
{
    static_assert((std::is_same<decltype(lrint((float)0)), long>::value), "");
    static_assert((std::is_same<decltype(lrint((bool)0)), long>::value), "");
    static_assert((std::is_same<decltype(lrint((unsigned short)0)), long>::value), "");
    static_assert((std::is_same<decltype(lrint((int)0)), long>::value), "");
    static_assert((std::is_same<decltype(lrint((unsigned int)0)), long>::value), "");
    static_assert((std::is_same<decltype(lrint((long)0)), long>::value), "");
    static_assert((std::is_same<decltype(lrint((unsigned long)0)), long>::value), "");
    static_assert((std::is_same<decltype(lrint((long long)0)), long>::value), "");
    static_assert((std::is_same<decltype(lrint((unsigned long long)0)), long>::value), "");
    static_assert((std::is_same<decltype(lrint((double)0)), long>::value), "");
    static_assert((std::is_same<decltype(lrintf(0)), long>::value), "");
    static_assert((std::is_same<decltype(lrint(Ambiguous())), Ambiguous>::value), "");
    assert(lrint(V(1)) == 1L);
    assert(lrint(V(1.)) == 1L);
#if CUDA_VERSION > 7050
    assert(lrint(V(1.f)) == 1L);
#endif
}

__device__ void test_lround()
{
    static_assert((std::is_same<decltype(lround((float)0)), long>::value), "");
    static_assert((std::is_same<decltype(lround((bool)0)), long>::value), "");
    static_assert((std::is_same<decltype(lround((unsigned short)0)), long>::value), "");
    static_assert((std::is_same<decltype(lround((int)0)), long>::value), "");
    static_assert((std::is_same<decltype(lround((unsigned int)0)), long>::value), "");
    static_assert((std::is_same<decltype(lround((long)0)), long>::value), "");
    static_assert((std::is_same<decltype(lround((unsigned long)0)), long>::value), "");
    static_assert((std::is_same<decltype(lround((long long)0)), long>::value), "");
    static_assert((std::is_same<decltype(lround((unsigned long long)0)), long>::value), "");
    static_assert((std::is_same<decltype(lround((double)0)), long>::value), "");
    static_assert((std::is_same<decltype(lroundf(0)), long>::value), "");
    static_assert((std::is_same<decltype(lround(Ambiguous())), Ambiguous>::value), "");
    assert(lround(V(1)) == 1L);
    assert(lround(V(1.)) == 1L);
    assert(lround(V(1.f)) == 1L);
}

__device__ void test_nan()
{
    static_assert((std::is_same<decltype(nan("")), double>::value), "");
    static_assert((std::is_same<decltype(nanf("")), float>::value), "");
}

__device__ void test_nearbyint()
{
    static_assert((std::is_same<decltype(nearbyint((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(nearbyint((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(nearbyint((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(nearbyint((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(nearbyint((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(nearbyint((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(nearbyint((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(nearbyint((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(nearbyint((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(nearbyint((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(nearbyintf(0)), float>::value), "");
    static_assert((std::is_same<decltype(nearbyint(Ambiguous())), Ambiguous>::value), "");
    assert(nearbyint(V(1)) == 1);
    assert(nearbyint(V(1.)) == 1);
    assert(nearbyint(V(1.f)) == 1);
    // There are more checks in test_rint(). rint and nearbyint behave the same
    // way on the GPU, so we only test them in one place.
}

__device__ void test_nextafter()
{
    static_assert((std::is_same<decltype(nextafter((float)0, (float)0)), float>::value), "");
    static_assert((std::is_same<decltype(nextafter((bool)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(nextafter((unsigned short)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(nextafter((float)0, (unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(nextafter((double)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(nextafter((int)0, (long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(nextafter((int)0, (unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(nextafter((double)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(nextafter((float)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(nextafterf(0,0)), float>::value), "");
    static_assert((std::is_same<decltype(nextafter((int)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(nextafter(Ambiguous(), Ambiguous())), Ambiguous>::value), "");
    //assert(nextafter(0,1) == hexfloat<double>(0x1, 0, -1074));

    // Invoke all our overloads.  Even though we don't check the exact result
    // (this is pretty annoying to do for this function), we make sure to *use*
    // the results so that these function calls can't be DCE'ed.
    assert(nextafter(V(0), 1) != 0);
    assert(nextafter(V(0), 1.) != 0);
    assert(nextafter(V(0), 1.f) != 0);

    assert(nextafter(V(0.), 1) != 0);
    assert(nextafter(V(0.), 1.) != 0);
    assert(nextafter(V(0.), 1.f) != 0);

    assert(nextafter(V(0.f), 1) != 0);
    assert(nextafter(V(0.f), 1.) != 0);
    assert(nextafter(V(0.f), 1.f) != 0);
}

__device__ void test_remainder()
{
    static_assert((std::is_same<decltype(remainder((float)0, (float)0)), float>::value), "");
    static_assert((std::is_same<decltype(remainder((bool)0, (float)0)), double>::value), "");
    static_assert((std::is_same<decltype(remainder((unsigned short)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(remainder((float)0, (unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(remainder((double)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(remainder((int)0, (long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(remainder((int)0, (unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(remainder((double)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(remainder((float)0, (double)0)), double>::value), "");
    static_assert((std::is_same<decltype(remainderf(0,0)), float>::value), "");
    static_assert((std::is_same<decltype(remainder((int)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(remainder(Ambiguous(), Ambiguous())), Ambiguous>::value), "");

    assert(std::remainder(V(1.5), 1) == -.5);
    assert(std::remainder(V(1.5), 1.) == -.5);
    assert(std::remainder(V(1.5), 1.f) == -.5);

    assert(std::remainder(V(1.5f), 1) == -.5);
    assert(std::remainder(V(1.5f), 1.) == -.5);
    assert(std::remainder(V(1.5f), 1.f) == -.5);

    assert(std::remainder(V(2), 1) == 0);
    assert(std::remainder(V(2), 1.) == 0);
    assert(std::remainder(V(2), 1.f) == 0);
}

__device__ void test_remquo()
{
    int ip;
    static_assert((std::is_same<decltype(remquo((float)0, (float)0, &ip)), float>::value), "");
    static_assert((std::is_same<decltype(remquo((bool)0, (float)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(remquo((unsigned short)0, (double)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(remquo((float)0, (unsigned int)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(remquo((double)0, (long)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(remquo((int)0, (long long)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(remquo((int)0, (unsigned long long)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(remquo((double)0, (double)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(remquo((float)0, (double)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(remquof(0,0, &ip)), float>::value), "");
    static_assert((std::is_same<decltype(remquo((int)0, (int)0, &ip)), double>::value), "");
    static_assert((std::is_same<decltype(remquo(Ambiguous(), Ambiguous(), &ip)), Ambiguous>::value), "");

    assert(std::remquo(V(1), 1, &ip) == 0);
    assert(std::remquo(V(1), 1., &ip) == 0);
    assert(std::remquo(V(1), 1.f, &ip) == 0);

    assert(std::remquo(V(0.5), 1, &ip) == 0.5);
    assert(std::remquo(V(0.5), 1., &ip) == 0.5);
    assert(std::remquo(V(0.5), 1.f, &ip) == 0.5);

    assert(std::remquo(V(0.5f), 1, &ip) == 0.5);
    assert(std::remquo(V(0.5f), 1., &ip) == 0.5);
    assert(std::remquo(V(0.5f), 1.f, &ip) == 0.5);
}

__device__ void test_rint_nearbyint()
{
    static_assert((std::is_same<decltype(rint((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(rint((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(rint((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(rint((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(rint((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(rint((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(rint((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(rint((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(rint((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(rint((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(rintf(0)), float>::value), "");
    static_assert((std::is_same<decltype(rint(Ambiguous())), Ambiguous>::value), "");
    // Verify that rint/nearbyint produce identical correct results
    auto check = [](double input, double fpresult) {
      // FP rint()/nearbyint must match the expected result.
      assert(rint(V(float(input))) == float(fpresult));
      assert(nearbyint(V(float(input))) == float(fpresult));
      assert(rint(V(input)) == fpresult);
      assert(nearbyint(V(input)) == fpresult);
      // for integral types, std::rint(input) == std::rint(double(input))
      int iinput = input;
      assert(std::rint(V(iinput)) == std::rint(double(V(iinput))));
      assert(std::nearbyint(V(iinput)) == std::nearbyint(double(V(iinput))));
    };
    // Whole values round to themselves and do not change sign.
    check(0.0, 0.0);
    check(-0.0, -0.0);
    check(1.0, 1.0);
    check(-1.0, -1.0);
    // Half-way values round towards nearest even number.
    check(2.5, 2.0);
    check(-2.5, -2.0);
    check(3.5, 4.0);
    check(-3.5, -4.0);
    // Everything else is rounded towards nearest integer.
    check(2.1, 2.0);
    check(-2.1, -2.0);
    check(2.7, 3.0);
    check(-2.7, -3.0);
    check(3.9, 4.0);
    check(-3.9, -4.0);
}

__device__ void test_round()
{
    static_assert((std::is_same<decltype(round((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(round((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(round((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(round((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(round((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(round((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(round((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(round((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(round((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(round((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(roundf(0)), float>::value), "");
    static_assert((std::is_same<decltype(round(Ambiguous())), Ambiguous>::value), "");
    assert(round(V(1)) == 1);
    assert(round(V(1.)) == 1);
    assert(round(V(1.f)) == 1);
}

__device__ void test_scalbln()
{
    static_assert((std::is_same<decltype(scalbln((float)0, (long)0)), float>::value), "");
    static_assert((std::is_same<decltype(scalbln((bool)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbln((unsigned short)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbln((int)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbln((unsigned int)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbln((long)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbln((unsigned long)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbln((long long)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbln((unsigned long long)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbln((double)0, (long)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalblnf(0, (long)0)), float>::value), "");
    static_assert((std::is_same<decltype(scalbln(Ambiguous(), Ambiguous())), Ambiguous>::value), "");
    assert(std::scalbln(V(1), 1) == 2);
    assert(std::scalbln(V(1), 1.) == 2);
    assert(std::scalbln(V(1), 1.f) == 2);

    assert(std::scalbln(V(1.), 1) == 2);
    assert(std::scalbln(V(1.), 1.) == 2);
    assert(std::scalbln(V(1.), 1.f) == 2);

    assert(std::scalbln(V(1.f), 1) == 2);
    assert(std::scalbln(V(1.f), 1.) == 2);
    assert(std::scalbln(V(1.f), 1.f) == 2);
}

__device__ void test_scalbn()
{
    static_assert((std::is_same<decltype(scalbn((float)0, (int)0)), float>::value), "");
    static_assert((std::is_same<decltype(scalbn((bool)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbn((unsigned short)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbn((int)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbn((unsigned int)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbn((long)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbn((unsigned long)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbn((long long)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbn((unsigned long long)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbn((double)0, (int)0)), double>::value), "");
    static_assert((std::is_same<decltype(scalbnf(0, (int)0)), float>::value), "");
    static_assert((std::is_same<decltype(scalbn(Ambiguous(), Ambiguous())), Ambiguous>::value), "");
    assert(std::scalbn(V(1), 1) == 2);
    assert(std::scalbn(V(1), 1.) == 2);
    assert(std::scalbn(V(1), 1.f) == 2);

    assert(std::scalbn(V(1.), 1) == 2);
    assert(std::scalbn(V(1.), 1.) == 2);
    assert(std::scalbn(V(1.), 1.f) == 2);

    assert(std::scalbn(V(1.f), 1) == 2);
    assert(std::scalbn(V(1.f), 1.) == 2);
    assert(std::scalbn(V(1.f), 1.f) == 2);
}

__device__ void test_tgamma()
{
    static_assert((std::is_same<decltype(tgamma((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(tgamma((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(tgamma((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(tgamma((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(tgamma((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(tgamma((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(tgamma((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(tgamma((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(tgamma((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(tgamma((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(tgammaf(0)), float>::value), "");
    static_assert((std::is_same<decltype(tgamma(Ambiguous())), Ambiguous>::value), "");
    assert(tgamma(V(1)) == 1);
    assert(tgamma(V(1.)) == 1);
    assert(tgamma(V(1.f)) == 1);
}

__device__ void test_trunc()
{
    static_assert((std::is_same<decltype(trunc((float)0)), float>::value), "");
    static_assert((std::is_same<decltype(trunc((bool)0)), double>::value), "");
    static_assert((std::is_same<decltype(trunc((unsigned short)0)), double>::value), "");
    static_assert((std::is_same<decltype(trunc((int)0)), double>::value), "");
    static_assert((std::is_same<decltype(trunc((unsigned int)0)), double>::value), "");
    static_assert((std::is_same<decltype(trunc((long)0)), double>::value), "");
    static_assert((std::is_same<decltype(trunc((unsigned long)0)), double>::value), "");
    static_assert((std::is_same<decltype(trunc((long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(trunc((unsigned long long)0)), double>::value), "");
    static_assert((std::is_same<decltype(trunc((double)0)), double>::value), "");
    static_assert((std::is_same<decltype(truncf(0)), float>::value), "");
    static_assert((std::is_same<decltype(trunc(Ambiguous())), Ambiguous>::value), "");
    assert(trunc(V(1)) == 1);
    assert(trunc(V(1.)) == 1);
    assert(trunc(V(1.f)) == 1);
}

__global__ void tests()
{
    test_abs();
    test_acos();
    test_asin();
    test_atan();
    test_atan2();
    test_ceil();
    test_cos();
    test_cosh();
    test_exp();
    test_fabs();
    test_floor();
    test_fmod();
    test_frexp();
    test_ldexp();
    test_log();
    test_log10();
    test_modf();
    test_pow();
    test_sin();
    test_sinh();
    test_sqrt();
    test_tan();
    test_tanh();
    test_signbit();
    test_fpclassify();
    test_isfinite();
    test_isnormal();
    test_isgreater();
    test_isgreaterequal();
    test_isinf();
    test_isless();
    test_islessequal();
    test_islessgreater();
    test_isnan();
    test_isunordered();
    test_acosh();
    test_asinh();
    test_atanh();
    test_cbrt();
    test_copysign();
    test_erf();
    test_erfc();
    test_exp2();
    test_expm1();
    test_fdim();
    test_fma();
    test_fmax();
    test_fmin();
    test_hypot();
    test_ilogb();
    test_lgamma();
    test_llrint();
    test_llround();
    test_log1p();
    test_log2();
    test_logb();
    test_lrint();
    test_lround();
    test_nan();
    test_nearbyint();
    test_nextafter();
    test_remainder();
    test_remquo();
    test_rint_nearbyint();
    test_round();
    test_scalbln();
    test_scalbn();
    test_tgamma();
    test_trunc();
}

int main() {
    tests<<<1,1>>>();
    cudaError_t err = cudaDeviceSynchronize();
    if (err != cudaSuccess) {
      printf("CUDA error %d\n", (int)err);
      return 1;
    }
    printf("Success!\n");
    return 0;
}


#else

#include <stdio.h>

// No C++11; test is a nop.
int main() {
  printf("Success!\n");
  return 0;
}

#endif // __cplusplus < 201103L
