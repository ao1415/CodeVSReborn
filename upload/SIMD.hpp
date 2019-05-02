#pragma once

#ifdef _MSC_VER
#include <intrin.h>

#define popcnt(a) __popcnt(a) 
#define pext(a, b) _pext_u32(a, b)

#else
#include <x86intrin.h>

#define popcnt(a) _popcnt32(a) 
#define pext(a, b) _pext_u32(a, b)

#endif
