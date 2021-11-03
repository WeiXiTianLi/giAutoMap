#include "QtWidgetsClassMySelectGenshinImpactHandle.h"

//#define 设计模式
#ifdef 设计模式

class HandleImpl
{
	//HWND inHandle;
public:
	HandleImpl() {}
	~HandleImpl() {}
	HWND outHandle = NULL;
	virtual bool tryGetHandle(HWND handle) = 0;
	virtual HWND getHandle() = 0;
};

class Handle_GenshinImpact : private HandleImpl
{
public:
	Handle_GenshinImpact() {}
	~Handle_GenshinImpact() {}
	bool tryGetHandle(HWND handle)
	{
		outHandle = handle;
		return IsWindow(outHandle);
	}
	HWND getHandle()
	{
		return outHandle;
	}
};
class Handle_TengXunYunYouXi :private HandleImpl
{
private:
	//HWND  (Handle_TengXunYunYouXi::ptr)(void);

	static HWND callbcakHandle;
	HWND(Handle_TengXunYunYouXi::* ptr)(void);
public:
	Handle_TengXunYunYouXi()
	{
		ptr = &Handle_TengXunYunYouXi::getHandle_Fail;
	}
	~Handle_TengXunYunYouXi()
	{

	}
public:
	bool tryGetHandle(HWND handle)
	{
		HWND parantHandle = GetNextWindow(handle, GW_HWNDNEXT);
		EnumChildWindows(parantHandle, (WNDENUMPROC)Handle_TengXunYunYouXi::EnumGameChildProc, NULL);
		outHandle = callbcakHandle;
		if (IsWindow(outHandle))
		{
			ptr = &Handle_TengXunYunYouXi::getHandle_Success;
			return true;
		}
		else
		{
			ptr = &Handle_TengXunYunYouXi::getHandle_Fail;
			return false;
		}
	}
	HWND getHandle()
	{
		return ((*this).*ptr)();
	}

private:
	static BOOL CALLBACK EnumGameChildProc(
		HWND handle,      // handle to child window
		LPARAM lpa    // application-defined value
	)
	{
		static int callbackCount = 0;

		callbackCount++;
		if (callbackCount == 2)
		{
			callbcakHandle = handle;
			return false;
		}
		return true;
	}

	HWND getHandle_Success()
	{
		return outHandle;
	}
	HWND getHandle_Fail()
	{
		return GetDesktopWindow();
	}
};
#else

HWND callbcakHandle;
int callbackCount;
static BOOL CALLBACK EnumGameChildProc(
	HWND handle,      // handle to child window
	LPARAM lpa    // application-defined value
)
{
	callbackCount++;
	if (callbackCount == 2)
	{
		callbcakHandle = handle;
		return false;
	}
	return true;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	//GetWindow
	char nameLis[256];
	GetWindowTextA(hwnd, nameLis, 256);
	if (string(nameLis) == "腾讯云游戏")
	{
		RECT r;
		GetClientRect(hwnd, &r);
		if (r.right - r.left > 1000 && r.bottom - r.top > 600)
		{
			callbcakHandle = hwnd;
		}
	}
	return true;
}

#endif

QtWidgetsClassMySelectGenshinImpactHandle::QtWidgetsClassMySelectGenshinImpactHandle(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	pos.x = 0;
	pos.y = 0;
}

QtWidgetsClassMySelectGenshinImpactHandle::~QtWidgetsClassMySelectGenshinImpactHandle()
{
}

void QtWidgetsClassMySelectGenshinImpactHandle::displayWndInfo()
{
	long long int nHandle = (long long int)giHandle;
	ui.lineEditName->setText(QString::fromLocal8Bit(giName.data()));
	ui.lineEditHandle->setText(QString::number(nHandle));
	ui.lineEditClass->setText(QString::fromLocal8Bit(giClass.data()));
}

void QtWidgetsClassMySelectGenshinImpactHandle::mouseMoveEvent(QMouseEvent * event)
{
	if (isStartSelect)
	{
		char classNameLis[256];
		char nameLis[256];

		pos.x = event->globalPos().x();
		pos.y = event->globalPos().y();

		giHandle = WindowFromPoint(pos);

		GetClassNameA(giHandle, classNameLis, 256);
		giClass = classNameLis;

		GetWindowTextA(giHandle, nameLis, 256);
		giName = nameLis;

		if (giName == "腾讯云游戏")
		{
#ifdef 设计模式
			Handle_TengXunYunYouXi TengXunYun;
			TengXunYun.tryGetHandle(giHandle);
			giHandle = TengXunYun.getHandle();
#else
			callbackCount = 0;
			EnumWindows(EnumWindowsProc, NULL);
			HWND parantHandle = callbcakHandle;//= GetNextWindow(callbcakHandle, GW_HWNDNEXT);
			EnumChildWindows(parantHandle, EnumGameChildProc, NULL);
			HWND outHandle = callbcakHandle;


			if (IsWindow(outHandle))
			{
				giHandle = outHandle;
			}
#endif // 设计模式
			GetClassNameA(giHandle, classNameLis, 256);
			giClass = classNameLis;

			GetWindowTextA(giHandle, nameLis, 256);
			giName = nameLis;
		}

		displayWndInfo();
	}
}

void QtWidgetsClassMySelectGenshinImpactHandle::mousePressEvent(QMouseEvent * event)
{
}

void QtWidgetsClassMySelectGenshinImpactHandle::mouseReleaseEvent(QMouseEvent * event)
{
	isStartSelect = false;
}

void QtWidgetsClassMySelectGenshinImpactHandle::SelectGenshinImpactWndStart()
{
	isStartSelect = true;
}

void QtWidgetsClassMySelectGenshinImpactHandle::SelectGenshinImpactWndOk()
{
	if (giHandle != NULL)
	{
		emit this->SendGenshinImpactWndHandleToATM(giHandle);
	}
	close();
}
