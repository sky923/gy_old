#ifndef _core_gyPlatformDetection_h_
#define _core_gyPlatformDetection_h_

namespace gy
{
// OS
#define __GY_ID_OS_UNKNOWN__ -1
#define __GY_ID_OS_WIN32__ 0
#define __GY_ID_OS_WIN64__ 1
#define __GY_ID_OS_WINSTORE__ 2
#define __GY_ID_OS_WINPHONE__ 3
#define __GY_ID_OS_MACOSX__ 1000
#define __GY_ID_OS_LINUX__ 2000
#define __GY_ID_OS_IOS__ 3000
#define __GY_ID_OS_IOS_SIMULATOR__ 3001
#define __GY_ID_OS_ANDROID__ 4000
#define __GY_ID_OS_ANDROID_ON_UNKNOWN__ 4001
#define __GY_ID_OS_ANDROID_ON_WINDOWS__ 4002
#define __GY_ID_OS_ANDROID_ON_MACOSX__ 4003
#define __GY_ID_OS_ANDROID_ON_LINUX__ 4004
#define __GY_ID_STR_OS_UNKNOWN__ "Unknown"
#define __GY_ID_STR_OS_WIN32__ "Windows x86"
#define __GY_ID_STR_OS_WIN64__ "Windows x64"
#define __GY_ID_STR_OS_WINSTORE__ "Windows Store"
#define __GY_ID_STR_OS_WINPHONE__ "Windows Phone"
#define __GY_ID_STR_OS_MACOSX__ "Mac OS X"
#define __GY_ID_STR_OS_LINUX__ "Linux"
#define __GY_ID_STR_OS_IOS__ "iOS"
#define __GY_ID_STR_OS_IOS_SIMULATOR__ "iOS Simulator"
#define __GY_ID_STR_OS_ANDROID__ "Android"
#define __GY_ID_STR_OS_ANDROID_ON_UNKNOWN__ "Android On Unknown OS"
#define __GY_ID_STR_OS_ANDROID_ON_WINDOWS__ "Android On Windows"
#define __GY_ID_STR_OS_ANDROID_ON_MACOSX__ "Android On Mac OS X"
#define __GY_ID_STR_OS_ANDROID_ON_LINUX__ "Android On Linux"

#if defined(WIN16) || defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#	if defined(ANDROID) || defined(__ANDROID__)
#		define __GY_ID_OS__ __GY_ID_OS_ANDROID_ON_WINDOWS__
#		define __GY_ID_STR_OS__ __GY_ID_STR_OS_ANDROID_ON_WINDOWS__
#		define __GY_OS_ENABLED_ANDROID__ 1
#		define __GY_OS_ENABLED_ANDROID_ON_WINDOWS__ 1
#	else
#		include <winapifamily.h>
#		if defined(WINAPI_FAMILY) && defined(WINAPI_FAMILY_APP) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
#				define __GY_ID_OS__ __GY_ID_OS_WINSTORE__
#				define __GY_ID_STR_OS__ __GY_ID_STR_OS_WINSTORE__
#				define __GY_OS_ENABLED_WINSTORE__ 1
#		elif defined(WINAPI_FAMILY) && defined(WINAPI_PARTITION_PHONE)
#				define __GY_ID_OS__ __GY_ID_OS_WINPHONE__
#				define __GY_ID_STR_OS__ __GY_ID_STR_OS_WINPHONE__
#				define __GY_OS_ENABLED_WINPHONE__ 1
#		else // #elif defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
#			if defined(WIN32) || defined(_WIN32)
#				define __GY_ID_OS__ __GY_ID_OS_WIN32__
#				define __GY_ID_STR_OS__ __GY_ID_STR_OS_WIN32__
#				define __GY_OS_ENABLED_WIN32__ 1
#			elif defined(WIN64) || defined(_WIN64)
#				define __GY_ID_OS__ __GY_ID_OS_WIN64__
#				define __GY_ID_STR_OS__ __GY_ID_STR_OS_WIN64__
#				define __GY_OS_ENABLED_WIN64__ 1
#			else
#				define __GY_ID_OS__ __GY_ID_OS_UNKNOWN__
#				define __GY_ID_STR_OS__ __GY_ID_STR_OS_UNKNOWN__
#				define __GY_OS_ENABLED_UNKNOWN__ 1
#			endif
#		endif
#		if defined(__GY_OS_ENABLED_WINSTORE__) || defined(__GY_OS_ENABLED_WINPHONE__)
#			define __GY_OS_ENABLED_WINAPP__
#		endif
#	endif
#elif defined(__APPLE__)
#	if defined(ANDROID) || defined(__ANDROID__)
#		define __GY_ID_OS__ __GY_ID_OS_ANDROID_ON_MACOSX__
#		define __GY_ID_STR_OS__ __GY_ID_STR_OS_ANDROID_ON_MACOSX__
#		define __GY_OS_ENABLED_ANDROID__ 1
#		define __GY_OS_ENABLED_ANDROID_ON_MACOSX__ 1
#	else
#		include <TargetConditionals.h>
#		if defined(TARGET_OS_IPHONE) && (TARGET_OS_IPHONE == 1)
#			define __GY_ID_OS__ __GY_ID_OS_IOS__
#			define __GY_ID_STR_OS__ __GY_ID_STR_OS_IOS__
#			define __GY_OS_ENABLED_IOS__ 1
#		elif defined(TARGET_IPHONE_SIMULATOR) && (TARGET_IPHONE_SIMULATOR == 1)
#			define __GY_ID_OS__ __GY_ID_OS_IOS_SIMULATOR__
#			define __GY_ID_STR_OS__ __GY_ID_STR_OS_IOS_SIMULATOR__
#			define __GY_OS_ENABLED_IOS_SIMULATOR__ 1
#			define __GY_OS_ENABLED_IOS__ 1
#		elif defined(TARGET_OS_MAC) && (TARGET_OS_MAC == 1)
#			define __GY_ID_OS__ __GY_ID_OS_MACOSX__
#			define __GY_ID_STR_OS__ __GY_ID_STR_OS_MACOSX__
#			define __GY_OS_ENABLED_MACOSX__ 1
#		else
#			define __GY_ID_OS__ __GY_ID_OS_UNKNOWN__
#			define __GY_ID_STR_OS__ __GY_ID_STR_OS_UNKNOWN__
#			define __GY_OS_ENABLED_UNKNOWN__ 1
#		endif
#	endif
#elif defined(linux) || defined(__linux) || defined(__linux__)
#	if defined(ANDROID) || defined(__ANDROID__)
#		define __GY_ID_OS__ __GY_ID_OS_ANDROID_ON_LINUX__
#		define __GY_ID_STR_OS__ __GY_ID_STR_OS_ANDROID_ON_LINUX__
#		define __GY_OS_ENABLED_ANDROID__ 1
#		define __GY_OS_ENABLED_ANDROID_ON_LINUX__ 1
#	else
#		define __GY_ID_OS__ __GY_ID_OS_LINUX__
#		define __GY_ID_STR_OS__ __GY_ID_STR_OS_LINUX__
#		define __GY_OS_ENABLED_LINUX__ 1
#	endif
#elif defined(ANDROID) || defined(__ANDROID__)
#		define __GY_ID_OS__ __GY_ID_OS_ANDROID__
#		define __GY_ID_STR_OS__ __GY_ID_STR_OS_ANDROID__
#		define __GY_OS_ENABLED_ANDROID__ 1
#		define __GY_OS_ENABLED_ANDROID_ON_UNKNOWN__ 1
#else
#	if defined(ANDROID) || defined(__ANDROID__)
#		define __GY_ID_OS__ __GY_ID_OS_ANDROID_ON_UNKNOWN__
#		define __GY_ID_STR_OS__ __GY_ID_STR_OS_ANDROID_ON_UNKNOWN__
#		define __GY_OS_ENABLED_ANDROID__ 1
#		define __GY_OS_ENABLED_ANDROID_ON_UNKNOWN__ 1
#	else
#		define __GY_ID_OS__ __GY_ID_OS_UNKNOWN__
#		define __GY_ID_STR_OS__ __GY_ID_STR_OS_UNKNOWN__
#		define __GY_OS_ENABLED_UNKNOWN__ 1
#	endif
#endif

// Compiler
#define __GY_COMPILER_UNKNOWN__ -1
#define __GY_COMPILER_GCC__ 0
#define __GY_COMPILER_GCC3_3__ (__GY_COMPILER_GCC__ + 1)
#define __GY_COMPILER_GCC3_4__ (__GY_COMPILER_GCC__ + 2)
#define __GY_COMPILER_GCC4_4__ (__GY_COMPILER_GCC__ + 3)
#define __GY_COMPILER_GCC4_5__ (__GY_COMPILER_GCC__ + 4)
#define __GY_COMPILER_GCC4_6__ (__GY_COMPILER_GCC__ + 5)
#define __GY_COMPILER_VC__ 1000
#define __GY_COMPILER_VC8__ (__GY_COMPILER_VC__ + 1)
#define __GY_COMPILER_VC9__ (__GY_COMPILER_VC__ + 2)
#define __GY_COMPILER_VC10__ (__GY_COMPILER_VC__ + 3)
#define __GY_COMPILER_VC11__ (__GY_COMPILER_VC__ + 4)

#if defined(__GNUC__)
#	define __GY_ID_COMPILER__ __GY_COMPILER_GCC__
#	define __GY_COMPILER_ENABLED_GCC__ 1
#	if __GNUC__ == 3 && (__GNUC_MINOR__ == 3 && __GNUC_PATCHLEVEL__ > 0)
#		define __GY_ID_COMPILER_VERSION__ __GY_COMPILER_GCC3_3__
#		define __GY_COMPILER_ENABLED_GCC3_3__ 1
#	elif __GNUC__ == 3 && (__GNUC_MINOR__ == 4 && __GNUC_PATCHLEVEL__ > 0)
#		define __GY_ID_COMPILER_VERSION__ __GY_COMPILER_GCC3_4__
#		define __GY_COMPILER_ENABLED_GCC3_4__ 1
#	elif __GNUC__ == 4 && (__GNUC_MINOR__ == 4 && __GNUC_PATCHLEVEL__ > 0)
#		define __GY_ID_COMPILER_VERSION__ __GY_COMPILER_GCC4_4__
#		define __GY_COMPILER_ENABLED_GCC4_4__ 1
#	elif __GNUC__ == 4 && (__GNUC_MINOR__ == 5 && __GNUC_PATCHLEVEL__ > 0)
#		define __GY_ID_COMPILER_VERSION__ __GY_COMPILER_GCC4_5__
#		define __GY_COMPILER_ENABLED_GCC4_5__ 1
#	elif __GNUC__ == 4 && (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)
#		define __GY_ID_COMPILER_VERSION__ __GY_COMPILER_GCC4_6__
#		define __GY_COMPILER_ENABLED_GCC4_6__ 1
#	else
#		define __GY_ID_COMPILER_VERSION__ __GY_COMPILER_UNKNOWN__
#	endif
#	if __GNUC__ >= 4 && __GNUC_MINOR__ >= 5
#		define __GY_COMPILER_IS_SUPPORTED_CPP11__
#	endif
#elif defined(_MSC_VER)
#	define __GY_ID_COMPILER__ __GY_COMPILER_VC__
#	define __GY_COMPILER_ENABLED_VC__ 1
#	if _MSC_VER == 1400
#		define __GY_ID_COMPILER_VERSION__ __GY_COMPILER_VC8__
#		define __GY_COMPILER_ENABLED_VC8__ 1
#	elif _MSC_VER == 1500
#		define __GY_ID_COMPILER_VERSION__ __GY_COMPILER_VC9__
#		define __GY_COMPILER_ENABLED_VC9__ 1
#	elif _MSC_VER == 1600
#		define __GY_ID_COMPILER_VERSION__ __GY_COMPILER_VC10__
#		define __GY_COMPILER_ENABLED_VC10__ 1
#	elif _MSC_VER > 1600 && _MSC_VER < 1800
#		define __GY_ID_COMPILER_VERSION__ __GY_COMPILER_VC11__
#		define __GY_COMPILER_ENABLED_VC11__ 1
#	else
#		define __GY_ID_COMPILER_VERSION__ __GY_COMPILER_UNKNOWN__
#	endif
#	if _MSC_VER >= 1600
#		define __GY_COMPILER_IS_SUPPORTED_CPP11__
#	endif
#else
#	define __GY_ID_COMPILER__ __GY_COMPILER_UNKNOWN__
#	define __GY_ID_COMPILER_VERSION__ __GY_COMPILER_UNKNOWN__
#endif

// NP(Null Pointer) (not NULL(0))
#ifndef NULL
#	define NULL 0
#endif
#ifdef __GY_COMPILER_IS_SUPPORTED_CPP11__
#	ifdef NP
#		undef NP
#	endif
#	define NP nullptr
#	define NullPtr NP
#	define NULL_PTR NP
#else // for compatiable with non C++11 (equal with NULL(0))
#	ifdef NP
#		undef NP
#	endif
#	define NP 0
#	define NullPtr NP
#	define NULL_PTR NP
#endif

// force inline
#ifdef FORCE_INLINE
#	undef FORCE_INLINE
#endif
#if !defined(_MSC_VER) && !defined(__GNUC__)
#	define FORCE_INLINE inline
#endif
#ifndef FORCE_INLINE
#	ifdef __GY_COMPILER_ENABLED_GCC__
#		define FORCE_INLINE __inline__
#	endif
#endif
#ifndef FORCE_INLINE
#	ifdef __GY_COMPILER_ENABLED_VC__
#		define FORCE_INLINE __forceinline
#	endif
#endif
}

#endif //_core_gyPlatformDetection_h_
