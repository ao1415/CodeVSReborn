#pragma once

#include <cstdint>

#define USE_AVX2

#ifdef USE_AVX2

// for SSE,AVX,AVX2
#include <immintrin.h>

#else

// for non-AVX2 : software emulation‚É‚æ‚épextÀ‘•(‚·‚±‚Ô‚é’x‚¢B‚Æ‚è‚ ‚¦‚¸“®‚­‚Æ‚¢‚¤‚¾‚¯B)
template<typename T> T pext(T val, T mask)
{
	T res = 0;
	size_t bitn = 0;
	for (size_t bit = 0; bit != sizeof(T) * 8; ++bit)
		if ((mask >> bit) & 1)
			res |= static_cast<T>((val >> bit) & 1) << bitn++;
	return res;
}

inline uint32_t _pext_u32(uint32_t a, uint32_t b) { return pext<uint32_t>(a, b); }
inline uint64_t _pext_u64(uint64_t a, uint64_t b) { return pext<uint64_t>(a, b); }

#endif
