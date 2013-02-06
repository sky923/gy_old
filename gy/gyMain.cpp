#include "gyMain.h"
#include <list>

//private:
gyMain *gyMain::m_instance;

// P.I.L에서 실행에 필수적인 함수를 gyMain클래스와 연결시킨다.
int lnk00() {return gyMain::getInstance()->onInitialize();}
int lnk01() {return gyMain::getInstance()->onCreate();}
int lnk02() {return gyMain::getInstance()->onPrepare();}
int lnk03() {return gyMain::getInstance()->onRun();}
int lnk04() {return gyMain::getInstance()->onUpdate();}
int lnk05() {return gyMain::getInstance()->onRender();}
int lnk06() {return gyMain::getInstance()->onFinalize();}

//protected:
gyMain::gyMain()
{
	m_instance = this;

	std::list<gy::_link_callback> callbacks;
	callbacks.push_back(gy::_link_callback(gy::__cb::A_APPLICATION_INITIALIZER, lnk00));
	callbacks.push_back(gy::_link_callback(gy::__cb::A_VIEW_INITIALIZER, lnk01));
	callbacks.push_back(gy::_link_callback(gy::__cb::A_APPLICATION_PREPARE, lnk02));
	callbacks.push_back(gy::_link_callback(gy::__cb::A_APPLICATION_EXECUTOR, lnk03));
	callbacks.push_back(gy::_link_callback(gy::__cb::A_APPLICATION_UPDATER, lnk04));
	callbacks.push_back(gy::_link_callback(gy::__cb::A_RENDERER_UPDATER, lnk05));
	callbacks.push_back(gy::_link_callback(gy::__cb::A_APPLICATION_FINALIZER, lnk06));
}

gyMain::gyMain(const gyMain &rhs)
{
	*this = rhs;
}

gyMain &gyMain::operator = (const gyMain &rhs)
{
	return *this;
}

//public:
gyMain::~gyMain()
{
}

gyMain *gyMain::getInstance()
{
	assert(m_instance);
	return m_instance;
}

//protected:
int gyMain::onRun()
{
	return gy::defaultExecuteApplication();
}

//public:
int gyMain::onInitialize()
{
	return gy::defaultInitializeApplication();
}

int gyMain::onFinalize()
{
	return gy::defaultFinalizeApplication();
}

int gyMain::onCreate()
{
	return gy::defaultInitializeView();
}

int gyMain::onPrepare()
{
	return Success;
}

int gyMain::onUpdate()
{
	return gy::defaultApplicationUpdate();
}

int gyMain::onRender()
{
	return gy::defaultRendererUpdate();
}