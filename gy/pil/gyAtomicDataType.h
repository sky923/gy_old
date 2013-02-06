#ifndef _core_sys_mvAtomicDataType_h_
#define _core_sys_mvAtomicDataType_h_

#include "gyPlatformDetection.h"
#include "gyDefaultDefinition.h"
#include <cstdio>
#include <cstdint>

// Min/Max type values for only Android NDK (current developed version : r8b)
//@todo: 모든 정의에 대하여 테스트되지 않았음 //gy
#if defined(__GY_OS_ENABLED_ANDROID__)
#define INT8_MIN        (-127 - 1)
#define SHORT_MIN       (-32768)
#define INT16_MIN       (-32767 - 1)
#define INT32_MIN       (-2147483647 - 1)
#define LONG64_MIN      (-9223372036854775807 - 1)
#define INT64_MIN       (-9223372036854775807 - 1)
#define INT128_MIN      (-170141183460469231731687303715884105727 - 1)

#define INT_PTR_MIN     (-2147483647 - 1)
#define LONG_PTR_MIN    (-2147483647 - 1)
#define PTRDIFF_T_MIN   (-2147483647 - 1)
#define SSIZE_T_MIN     (-2147483647 - 1)

#define INT8_MAX        127
#define UINT8_MAX       0xff
#define BYTE_MAX        0xff
#define SHORT_MAX       32767
#define INT16_MAX       32767
#define USHORT_MAX      0xffff
#define UINT16_MAX      0xffff
#define WORD_MAX        0xffff
#define INT32_MAX       2147483647
#define UINT32_MAX      0xffffffff
#define DWORD_MAX       0xffffffff
#define LONG64_MAX      9223372036854775807
#define INT64_MAX       9223372036854775807
#define DWORDLONG_MAX   0xffffffffffffffff
#define ULONG64_MAX     0xffffffffffffffff
#define DWORD64_MAX     0xffffffffffffffff
#define UINT64_MAX      0xffffffffffffffff
#define INT128_MAX      170141183460469231731687303715884105727
#define UINT128_MAX     0xffffffffffffffffffffffffffffffff

#undef SIZE_T_MAX

#define INT_PTR_MAX     2147483647 
#define UINT_PTR_MAX    0xffffffff
#define LONG_PTR_MAX    2147483647
#define ULONG_PTR_MAX   0xffffffff
#define DWORD_PTR_MAX   0xffffffff
#define PTRDIFF_T_MAX   2147483647
#define SIZE_T_MAX      0xffffffff
#define SSIZE_T_MAX     2147483647
#define _SIZE_T_MAX     0xffffffff
#endif // defined(__GY_OS_ENABLED_ANDROID__)

namespace gy
{
	namespace type
	{
	typedef void *void_ptr;
	typedef bool *bool_ptr;
	typedef char *char_ptr;
	typedef unsigned char uchar;
	typedef unsigned char *uchar_ptr;
	typedef short *short_ptr;
	typedef unsigned short ushort;
	typedef unsigned short *ushort_ptr;
	typedef int *int_ptr;
	typedef unsigned int uint; 
	typedef unsigned int *uint_ptr;
	typedef long *long_ptr;
	typedef unsigned long ulong;
	typedef unsigned long *ulong_ptr;
	typedef float *float_ptr;
	typedef double *double_ptr;

// long long
#if defined(WIN32) || defined(WIN64) || defined(__GNUC__)
	#define HAVE_LONG_LONG 1
#endif

//@todo: little endian (현재 적용한 곳 없음)//gy
#if defined(WIN32) || defined(WIN64) || defined(LINUX32)
	#define IS_LITTLE_ENDIAN 1
#else
	#define IS_BIG_ENDIAN 1
#endif

// check default type
#if defined(WIN32) || defined(WINIA32) || defined(LINUX32) || defined(LINUXIA32) || defined(SOLARIS32) || defined(HPUX32) || defined(HPUXIA32) || defined(AIX32)
	#define __SIZE_OF_BYTE__ 1L
	#define __SIZE_OF_SHORT__ 2L
	#define __SIZE_OF_INT__ 4L
	#define __SIZE_OF_INT64__ 8L
	#define __SIZE_OF_LONG__ 4L
	#define __SIZE_OF_FLOAT__ 4L
	#define __SIZE_OF_DOUBLE__ 8L
	#define __SIZE_OF_WORD__ 4L
	#define __SIZE_OF_DWORD__ 8L
#else
	#define __SIZE_OF_BYTE__ 1L
	#define __SIZE_OF_SHORT__ 2L
	#define __SIZE_OF_INT__ 4L
	#define __SIZE_OF_INT64__ 8L
	#define __SIZE_OF_LONG__ 8L
	#define __SIZE_OF_FLOAT__ 4L
	#define __SIZE_OF_DOUBLE__ 8L
	#define __SIZE_OF_WORD__ 4L
	#define __SIZE_OF_DWORD__ 8L
#endif

// type redefination for each platform
// int32
#if __SIZE_OF_INT__ == 4L
	typedef unsigned int uint32;
	typedef uint32 * uint32_ptr;
	typedef int int32;
	typedef int *int32_ptr;
	#define _INT32(n) n
	#define _UINT32(n) n ## U
#elif __SIZE_OF_LONG__ == 4L
	typedef unsigned long uint32;
	typedef uint32 *uint32_ptr;
	typedef long int32;
	typedef int32 *int32_ptr;
	#define _INT32(n) n ## L
	#define _UINT32(n) n ## UL
#else
#error No suitable type for uint32/int32
#endif

// int16
#if __SIZE_OF_SHORT__ == 2L
	typedef short int16;
	typedef int16 *int16_ptr;
	typedef unsigned short uint16;
	typedef uint16 *uint16_ptr;
#else
	#error No suitable type for uint16/int16
#endif

// int8
#if __SIZE_OF_BYTE__ == 1L
	typedef char int8;
	typedef int8 *int8_ptr;
	typedef unsigned char uint8;
	typedef uint8 *uint8_ptr;
#else
	#error No suitable type for uint16/int16
#endif

// int64
#ifdef HAVE_LONG_LONG
	#if __SIZE_OF_LONG__ == 8L
		typedef long int64;
		typedef int64 *int64_ptr;
#ifdef __APPLE__
#define _UINT64
#endif
		typedef unsigned long uint64;
		typedef uint64 *uint64_ptr;
#elif defined(WIN16)
		typedef __int64 int64;
		typedef int64 *int64_ptr;
		typedef unsigned __int64 uint64;
		typedef uint64 *uint64_ptr;
	#elif defined(WIN32) && !defined(__GNUC__)
		typedef __int64 int64;
		typedef int64 *int64_ptr;
		typedef unsigned __int64 uint64;
		typedef uint64 *uint64_ptr;
		#define _INT64(n) n ## i64
		#define _UINT64(n) n ##s ui64
	#else
		typedef long long int64;
		typedef int64 *int64_ptr;
		typedef unsigned long long uint64;
		typedef uint64 *uint64_ptr;
		#define _INT64(n) n ## LL
		#define _UINT64(n) n ## ULL
	#endif
#else
	typedef struct {
	#ifdef IS_LITTLE_ENDIAN
		uint32 lo, hi;
	#else
		uint32 hi, lo;
	#endif
	} int64;
	typedef int64 *int64_ptr;
	typedef int64 uint64;
	typedef uint64 *uint64_ptr;
#endif

	typedef int id_t;
	}

	using namespace type;

	template <typename _Type_> inline bool make_bool(_Type_ value) {return value == 0 ? false : true;}
	inline unsigned int make_uint(bool value) {return value == true ? 1 : 0;}
	inline int make_int(bool value) {return value == true ? 1 : 0;}
}

using namespace gy::type;

#endif //_core_sys_mvAtomicDataType_h_
