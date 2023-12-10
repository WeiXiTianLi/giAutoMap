#include "ATM_GiState.h"

using namespace std;
using namespace cv;
bool ATM_GiState::isRun()
{
	isRunning = IsWindow(giHandle);
	//if (giName != "ԭ��")
	//{
	//	isRunning = true;
	//}
	return isRunning;
}

void ATM_GiState::getHandle()
{
	if (giName != "ԭ��")
	{
		getRect();
		return;
	}
	giHandle = FindWindowA(giWndClass.c_str(), giName.c_str());
	if (giHandle == NULL)
	{
		LPCWSTR giWindowName = { L"ԭ��" };
		if (giHandle == NULL)
		{
			giWindowName = L"Genshin Impact";
			giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* ƥ�����ƣ�ԭ�� */
		}
		if (giHandle == NULL)
		{
			giWindowName = L"\u539F\u795E";
			giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* ƥ�����ƣ�ԭ�� */
		}
		if (giHandle == NULL)
		{
			giWindowName = L"\uC6D0\uC2E0";
			giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* ƥ�����ƣ�?? */
		}
	}
#ifdef _DEBUG
	//cout << "giHandle: " << giHandle << endl;
#endif
	getRect();
}

void ATM_GiState::getRect()
{
	if (giHandle != NULL)
	{
		GetWindowRect(giHandle, &giRect);
		GetClientRect(giHandle, &giClientRect);
		int x_offset = GetSystemMetrics(SM_CXDLGFRAME);
		int y_offset = GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION);

		/*SetWindowPos(
			hwnd,
			0,
			margins_xy.cxLeftWidth + (isNoBorder ? 0 : x_offset),
			margins_xy.cxRightWidth + (isNoBorder ? 0 : y_offset),
			margins_size.cyTopHeight,
			margins_size.cyBottomHeight,
			SWP_NOACTIVATE | SWP_FRAMECHANGED
			);
		*/
		giSize.width = giClientRect.right - giClientRect.left;// giRect.right - giRect.left;//+6
		giSize.height = giClientRect.bottom - giClientRect.top;//giRect.bottom - giRect.top;//+29

		getGiRectMode();
	}
	else
	{
		giRect.left = 100;
		giRect.top = 100;
		giRectMode = RectMode::FW_MINIMIZE;
	}
}

int ATM_GiState::getGiRectMode()
{

	if (giHandle != NULL)
	{
		const Size size1920x1080 = Size(1920, 1080);
		const Size size1680x1050 = Size(1650, 1080);


		if (giFrame.cols == 1920)
		{
			isFullScreen = true;
			giSize = size1920x1080;
			giRectMode = RectMode::FW_1920x1080;
			return giRectMode;
		}
		if (giFrame.rows == 1080)
		{
			isFullScreen = true;
			giSize = size1920x1080;
			giRectMode = RectMode::FW_1920x1080;
			return giRectMode;
		}
		giRectMode = RectMode::FW_UNDIFINDE;
	}
	else
	{
		giRectMode = RectMode::FW_MINIMIZE;
	}
	return giRectMode;
}

int ATM_GiState::getGiState()
{
	//true ? getHandle() : giRectMode = RectMode::FW_MINIMIZE;
	isRun() ? getHandle() : giRectMode = RectMode::FW_MINIMIZE;
	return giRectMode;
}
Point ATM_GiState::getOffset()
{
	cv::Point res;
	switch (giRectMode)
	{
	case FW_1920x1080:
	{
		res.x = 288;
		res.y = 82;
		break;
	}
	case FW_1680x1050:
	{
		res.x = 250;
		res.y = 82;
		break;
	}

	default:
	{

		break;

	}
	}
	return res;
}

//ȫ������
void ATM_GiState::getAllScreen()
{
	static HBITMAP	hBmp;
	BITMAP bmp;

	DeleteObject(hBmp);

	if (giHandle == NULL)return;

	//��ȡĿ�����Ĵ��ڴ�СRECT
	//GetWindowRect(giHandle, &giRect);/* ��ԭ�񴰿ڵĲ��� */

	//��ȡĿ������DC
	HDC hScreen = CreateDCA("DISPLAY", NULL, NULL, NULL);//GetDC(giHandle);/* ��ԭ�񴰿ڵĲ��� */
	//Ϊ��Ļ�豸�����������ݵ��ڴ��豸������   
	HDC hCompDC = CreateCompatibleDC(hScreen);

	giRect.left = 0;
	giRect.top = 0;
	giRect.right = GetDeviceCaps(hScreen, HORZRES);
	giRect.bottom = GetDeviceCaps(hScreen, VERTRES);

	//��ȡĿ�����Ŀ�Ⱥ͸߶�
	int	nWidth = giRect.right - giRect.left;
	int	nHeight = giRect.bottom - giRect.top;

	//����Bitmap����
	hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//�õ�λͼ

	SelectObject(hCompDC, hBmp); //��д��ȫ��
	BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);

	//�ͷŶ���
	DeleteDC(hScreen);
	DeleteDC(hCompDC);

	//����ת��
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;

	//mat����
	giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));

	GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, giFrame.data);

}

void ATM_GiState::getGiScreen()
{
	static HBITMAP	hBmp;
	BITMAP bmp;

	DeleteObject(hBmp);

	if (giHandle == NULL)return;

	//��ȡĿ�����Ĵ��ڴ�СRECT
	//GetWindowRect(giHandle, &giRect);/* ��ԭ�񴰿ڵĲ��� */
	GetClientRect(giHandle, &giRect);/* ��ԭ�񴰿ڵĲ��� */

	//��ȡĿ������DC
	HDC hScreen = GetDC(giHandle);/* ��ԭ�񴰿ڵĲ��� */
	HDC hCompDC = CreateCompatibleDC(hScreen);

	//��ȡĿ�����Ŀ�Ⱥ͸߶�
	int	nWidth = giRect.right - giRect.left;
	int	nHeight = giRect.bottom - giRect.top;

	//����Bitmap����
	hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//�õ�λͼ

	SelectObject(hCompDC, hBmp); //��д��ȫ��
	BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);

	//�ͷŶ���
	DeleteDC(hScreen);
	DeleteDC(hCompDC);

	//����ת��
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;

	//mat����
	giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));

	GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, giFrame.data);

}

void ATM_GiState::reSizeFrame()
{
	int& x = giFrame.cols, & y = giFrame.rows;
	double f = 1, fx = 1, fy = 1;

	if (static_cast<double>(x) / static_cast<double>(y) == 16.0 / 9.0)
	{

		//��������������
		if (x != 1920 && y != 1080)
		{
			cv::resize(giFrame, giFrame, cv::Size(1920, 1080));
		}
	}
	else if (static_cast<double>(x) / static_cast<double>(y) > 16.0 / 9.0)
	{

		//���ͣ��Կ�Ϊ����

		// x = (y * 16) / 9;
		f = y / 1080.0;
		//��giFrame���ŵ�1920*1080�ı���
		fx = x / f;
		// ��ͼƬ����
		cv::resize(giFrame, giFrame, cv::Size(static_cast<int>(fx), 1080));

	}
	else if (static_cast<double>(x) / static_cast<double>(y) < 16.0 / 9.0)
	{

		//���ͣ��Ը�Ϊ����

		// x = (y * 16) / 9;
		f = x / 1920.0;
		//��giFrame���ŵ�1920*1080�ı���
		fy = y / f;
		// ��ͼƬ����
		cv::resize(giFrame, giFrame, cv::Size(1920, static_cast<int>(fy)));
	}
	else
	{
		//����

	}
}

void ATM_GiState::getGiScreen2()
{
	static HBITMAP	hBmp;
	BITMAP bmp;

	DeleteObject(hBmp);

	//cout << "getGiScreen2: " << giHandle << endl;

	if (giHandle == NULL)return;

	//��ȡĿ�����Ĵ��ڴ�СRECT
	//GetWindowRect(giHandle, &giRect);/* ��ԭ�񴰿ڵĲ��� */
	GetClientRect(giHandle, &giRect);/* ��ԭ�񴰿ڵĲ��� */

	//��ȡĿ������DC
	HDC hScreen = GetDC(giHandle);/* ��ԭ�񴰿ڵĲ��� */
	HDC hCompDC = CreateCompatibleDC(hScreen);

	//��ȡĿ�����Ŀ�Ⱥ͸߶�
	int	nWidth = giRect.right - giRect.left;
	int	nHeight = giRect.bottom - giRect.top;

	//����Bitmap����
	hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//�õ�λͼ

	SelectObject(hCompDC, hBmp); //��д��ȫ��
	BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);

	//�ͷŶ���
	DeleteDC(hScreen);
	DeleteDC(hCompDC);

	//����ת��
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;

	//mat����
	giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));

	GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, giFrame.data);

	if (giFrame.channels() == 3)
	{
		cvtColor(giFrame, giFrame, CV_RGB2RGBA);
	}
}
void ATM_GiState::getGiFrame()
{
	if (isAutoScreen)
	{
		if (giWndClass == "UnityWndClass")
		{
			getGiScreen();
		}
		else
		{
			getGiScreen2();

		}
		//else
		//{
		//	getScreen(giHandle)
		//}
	}
	else
	{
		getAllScreen();
	}

	reSizeFrame();

	getGiRectMode();
	if (giRectMode > 0)
	{
		getGiFramePaimon();
		getGiFrameMap();
		getGiFrameUID();
	}
#ifdef _DEBUG
	//cout << "Gi Window Rect Mode: "<<giRectMode << endl;
#endif
}

void ATM_GiState::getGiFramePaimon()
{
	Rect PaimonRect;
	switch (giRectMode)
	{
	case FW_1920x1080:
	{
		PaimonRect = Rect(26, 12, 68, 77);
		resIdPaimon = 0;
		break;
	}
	default:
	{

		PaimonRect = Rect(cvCeil(giSize.width * 0.0135), cvCeil(giSize.width * 0.006075), cvCeil(giSize.width * 0.035), cvCeil(giSize.width * 0.0406));
		resIdPaimon = 0;
		// ���������
		if (giSize.width / giSize.height == 64 / 27)
		{
			PaimonRect = Rect(cvCeil(giSize.width * 0.038), cvCeil(giSize.height * 0.012), cvCeil(giSize.height / 9.0 * 16.0 * 0.035), cvCeil(giSize.height / 9.0 * 16.0 * 0.0406));
		}
		break;
	}
	}
	giFrame(PaimonRect).copyTo(giFramePaimon);

}

void ATM_GiState::getGiFrameMap()
{
	Rect mapRect;
	switch (giRectMode)
	{
	case FW_1920x1080:
	{
		mapRect = Rect(62, 19, 212, 212);
		break;
	}
	default:
	{
		mapRect = Rect(cvCeil(giSize.width * 0.032), cvCeil(giSize.width * 0.01), cvCeil(giSize.width * 0.11), cvCeil(giSize.width * 0.11));
		if (giSize.width / giSize.height == 64 / 27)
		{
			mapRect = Rect(cvCeil(giSize.width * 0.051), cvCeil(giSize.height / 9.0 * 16.0 * 0.01), cvCeil(giSize.height / 9.0 * 16.0 * 0.11), cvCeil(giSize.height / 9.0 * 16.0 * 0.11));
		}
		break;
	}
	}
	giFrame(mapRect).copyTo(giFrameMap);
}

void ATM_GiState::getGiFrameUID()
{
	Rect uidRect;
	switch (giRectMode)
	{
	case FW_1920x1080:
	{
		uidRect = Rect(giFrame.cols - 240, giFrame.rows - 25, 180, 18);
		break;
	}
	default:
	{
		uidRect = Rect(giFrame.cols - 240, giFrame.rows - 25, 180, 18);
		break;
	}
	}
	giFrame(uidRect).copyTo(giFrameUID);
}

void ATM_GiState::setGiNameClass(LANGID SystemLanguageID)
{
	switch (SystemLanguageID)
	{
	case 0X0804:
	{
		giName = "ԭ��";
		break;
	}
	case 0x0409:
	{
		giName = "Genshin Impact";
		break;
	}
	default:
	{
		giName = "Genshin Impact";//GenshinImpact.
		break;
	}
	}
}

void ATM_GiState::setGiHandle(HWND GiHandle)
{
	if (GiHandle != NULL)
	{
		char classNameLis[256];
		char nameLis[256];
		//cout << GiHandle << endl;
		giHandle = GiHandle;
		GetClassNameA(GiHandle, classNameLis, 256);
		giWndClass = classNameLis;
		GetWindowTextA(GiHandle, nameLis, 256);
		giName = nameLis;
	}
}
