#ifdef __ANDROID__
#include <android_native_app_glue.h>
#endif
#include "gyMainCore.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <cassert>
#include "gyView.h"


/// MAIN_RET_FAILED
#define MAIN_RET_FAILED(r) if (r == Failed) return r;
/// r = return value, cf = customized function, df = default function
#define CUSTOM_FUNC_SELECTOR(r, cf, df) r = cf != NULL ? cf() : df;
#define CALLBACK_FUNC_SELECTOR(cf, df) cf != NULL ? cf : df;
namespace gy
{
	namespace type {namespace __cb {int __do_nothing() {return Success;}}}
	using namespace type::__cb;
	// 콜백 관리
	type::default_callback_t bridge_function_of_application_initializer = NULL;
	type::default_callback_t bridge_function_of_view_initializer = NULL;
	type::default_callback_t bridge_function_of_application_pre_executor = NULL;
	type::default_callback_t bridge_function_of_application_executor = NULL;
	type::default_callback_t bridge_function_of_application_finalizer = NULL;
	type::default_callback_t bridge_function_of_renderer_updater = NULL;
	type::default_callback_t bridge_function_of_application_updater = NULL;
	_link_callback::_link_callback(int callbackType, type::default_callback_t callbackFunction) 
	{
		if (callbackFunction == NULL)
		{
			assert(false && "it have to need a valid callback function. type of \"int (*)()\"");
			return;
		}

		switch (callbackType)
		{
		case A_APPLICATION_INITIALIZER:
			bridge_function_of_application_initializer = callbackFunction;
			break;

		case A_VIEW_INITIALIZER:
			bridge_function_of_view_initializer = callbackFunction;
			break;

		case A_APPLICATION_PREPARE:
			bridge_function_of_application_pre_executor = callbackFunction;
			break;

		case A_APPLICATION_EXECUTOR:
			bridge_function_of_application_executor = callbackFunction;
			break;

		case A_APPLICATION_FINALIZER:
			bridge_function_of_application_finalizer = callbackFunction;
			break;

		case A_RENDERER_UPDATER:
			bridge_function_of_renderer_updater = callbackFunction;
			break;

		case A_APPLICATION_UPDATER:
			bridge_function_of_application_updater = callbackFunction;
			break;

		default:
			assert(false && "it have to need a valid callback type.");
		}
	}

	type::default_callback_t getCallback(int callbackType)
	{
		switch (callbackType)
		{
		case A_APPLICATION_INITIALIZER:
			return CALLBACK_FUNC_SELECTOR(bridge_function_of_application_initializer, defaultInitializeApplication);

		case A_VIEW_INITIALIZER:
			return CALLBACK_FUNC_SELECTOR(bridge_function_of_view_initializer, defaultInitializeView);

		case A_APPLICATION_PREPARE:
			return CALLBACK_FUNC_SELECTOR(bridge_function_of_application_pre_executor, __do_nothing);

		case A_APPLICATION_EXECUTOR:
			return CALLBACK_FUNC_SELECTOR(bridge_function_of_application_executor, defaultExecuteApplication);

		case A_APPLICATION_FINALIZER:
			return CALLBACK_FUNC_SELECTOR(bridge_function_of_application_finalizer, defaultFinalizeApplication);

		case A_RENDERER_UPDATER:
			return CALLBACK_FUNC_SELECTOR(bridge_function_of_renderer_updater, defaultRendererUpdate);

		case A_APPLICATION_UPDATER:
			return CALLBACK_FUNC_SELECTOR(bridge_function_of_application_updater, defaultApplicationUpdate);

		default:
			assert(false && "it have to need a valid callback type.");
		}

		return NullPtr;
	}

	// 프로세스 아큐먼트 관리
	unsigned long argc = 0;
	char **argv = {0,};
	const int getArgumentSize() {return argc;}
	const char *getArgument(unsigned long index)
	{
		if (index < 0 || index >= argc)
		{
			return NULL;
		}

		return argv[index];
	}
	void setArgumentSize(const int argSize)
	{
		argc = argSize;
	}
	void setArgument(char **arg)
	{
		argv = arg;
	}

	int __execute()
	{
		int ret = Success;
		// 어플리케이션 초기화
		{
			type::default_callback_t applicationInitializer = getCallback(A_APPLICATION_INITIALIZER);
			if (applicationInitializer != NULL) ret = applicationInitializer();
			MAIN_RET_FAILED(ret);
		}

		// 뷰 초기화
		{
			type::default_callback_t viewInitializer = getCallback(A_VIEW_INITIALIZER);
			if (viewInitializer != NULL) ret = viewInitializer();
			MAIN_RET_FAILED(ret);
		}

		// 어플리케이션 실행
		{
			type::default_callback_t applicationExecutor = getCallback(A_APPLICATION_EXECUTOR);
			if (applicationExecutor != NULL) ret = applicationExecutor();
			MAIN_RET_FAILED(ret);
		}

		// 어플리케이션 종료시
		{
			type::default_callback_t applicationFinalizer = getCallback(A_APPLICATION_FINALIZER);
			if (applicationFinalizer != NULL) ret = applicationFinalizer();
			MAIN_RET_FAILED(ret);
		}

		return Success;
	}

	int executeMain(void *arg)
	{
#if defined(__GY_OS_ENABLED_ANDROID__)
		gy::setMainView((view_t)arg);
#endif
		return __execute();
	}

#if defined(__GY_OS_ENABLED_WINSTORE__) || defined(__GY_OS_ENABLED_WINPHONE__)
	int executeMain(Platform::Array<Platform::String^> ^__arg)
	{
		gy::setArgumentSize((size_t)__arg->Length);
		const size_t argc = (size_t)gy::getArgumentSize();
		char **argv = new char *[argc];
		for (size_t i = 0; i < argc; ++i)
		{
			size_t len = __arg[i]->Length();
			argv[i] = new char[len + 1];
			wcstombs_s(&len, argv[i], len + 1, __arg[i]->Data(), len + 1);
		}
		gy::setArgument(argv);
		return __execute();
	}
#endif

	int executeMain(const int __argc, const char **__argv)
	{
		gy::setArgumentSize((size_t)__argc);
		const size_t argc = (size_t)gy::getArgumentSize();
		char **argv = new char *[argc];
		for (size_t i = 0; i < argc; ++i)
		{
			const size_t len = strlen(__argv[i]);
			argv[i] = new char[len + 1];
#if defined(__GY_OS_ENABLED_WIN32__) || defined(__GY_OS_ENABLED_WIN64__) || defined(__GY_OS_ENABLED_WINSTORE__) || defined(__GY_OS_ENABLED_WINPHONE__)
			strcpy_s(&argv[i][0], len + 1, &__argv[i][0]);
#else
			strcpy(&argv[i][0], &__argv[i][0]);
#endif
		}
		gy::setArgument(argv);
		return __execute();
	}
}
