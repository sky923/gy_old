#ifndef __gy_pil_gyMainCore_h__
#define __gy_pil_gyMainCore_h__

#include "gyPlatformDetection.h"

namespace gy
{
	// 안드로이드, 혹은 iOS가 아닌 데스크톱 플랫폼의 경우, 프로세스의 아큐먼트들을 얻어낼 수 있다.
#if !defined(__GY_OS_ENABLED_ANDROID__) || !defined(__GY_OS_ENABLED_IOS__)
	const int getArgumentSize();
	const char *getArgument(unsigned long index);
	void setArgumentSize(const int argSize);
	void setArgument(char **arg);
	int executeMain(void *arg);
#if defined(__GY_OS_ENABLED_WINSTORE__) || defined(__GY_OS_ENABLED_WINPHONE__)
	int executeMain(Platform::Array<Platform::String^> ^__arg);
#endif
	int executeMain(const int argc, const char **argv);
#endif

	// 프로세스 시작점 관리
	namespace type
	{
		namespace __cb
		{
			enum
			{
				A_APPLICATION_INITIALIZER = 1000,
				A_VIEW_INITIALIZER,
				A_APPLICATION_PREPARE,
				A_APPLICATION_EXECUTOR,
				A_APPLICATION_FINALIZER,
				A_RENDERER_UPDATER,
				A_APPLICATION_UPDATER,
			};
		}

		typedef int (*default_callback_t)();
	}

	struct _link_callback
	{
		_link_callback(int callbackType, type::default_callback_t arg0);
	};

	type::default_callback_t getCallback(int callbackType);
}

#if defined(__GY_OS_ENABLED_ANDROID__)
#	define The_Main void android_main(struct android_app *__arg)
#	define gyExecute() app_dummy(); exit(gy::executeMain(__arg));
#elif defined(__GY_OS_ENABLED_IOS__)
#	define The_Main int main(size_t __argc, const char **__argv)
#	define gyExecute() return gy::executeMain(__argc, __argv);
#elif defined(__GY_OS_ENABLED_WIN32__) || defined(__GY_OS_ENABLED_WIN64__)
#	if defined(_CONSOLE)
#	define The_Main int main(size_t __argc, const char **__argv)
#	else
#	include <windows.h>
#	define The_Main int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
#	endif
#	define gyExecute() return gy::executeMain(__argc, __argv);
#elif defined(__GY_OS_ENABLED_WINSTORE__) || defined(__GY_OS_ENABLED_WINPHONE__)
#include <wrl.h>
#include <wrl/client.h>
#	define The_Main using namespace Windows::ApplicationModel;\
using namespace Windows::ApplicationModel::Core;\
using namespace Windows::ApplicationModel::Activation;\
using namespace Windows::UI::Core;\
using namespace Windows::System;\
using namespace Windows::Foundation;\
[Platform::MTAThread]\
int main(Platform::Array<Platform::String^> ^__arg) 
#	define gyExecute() return gy::executeMain(__arg);
#elif defined(__GY_OS_ENABLED_MACOSX__)
#	define The_Main int main(int __argc, const char **__argv)
#	define gyExecute() return gy::executeMain(__argc, __argv);
#elif defined(__GY_OS_ENABLED_LINUX__)
#	define The_Main int main(int __argc, const char **__argv)
#	define gyExecute() return gy::executeMain(__argc, __argv);
#endif
#define Is_Here {gyExecute();}

#define GY_FRIEND_WITH_LINKING_API() \
	friend int lnk00(); \
	friend int lnk01(); \
	friend int lnk02(); \
	friend int lnk03(); \
	friend int lnk04(); \
	friend int lnk05(); \
	friend int lnk06(); \
	friend int lnk07(); \
	friend int lnk08(); \
	friend int lnk09();
#endif