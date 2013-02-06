#ifndef __gy_gyMain__
#define __gy_gyMain__

#include "pil/gyPIL.h"

class gyMain
{
private:
	GY_FRIEND_WITH_LINKING_API();
	static gyMain *m_instance;

protected:
	gyMain();
	gyMain(const gyMain &rhs);
	gyMain &operator = (const gyMain &rhs);
public:
	virtual ~gyMain();
	static gyMain *getInstance();

protected:
	virtual int onRun();
	virtual int onInitialize();
	virtual int onFinalize();
	virtual int onCreate();
	virtual int onPrepare();
	virtual int onUpdate();
	virtual int onRender();
};

#endif