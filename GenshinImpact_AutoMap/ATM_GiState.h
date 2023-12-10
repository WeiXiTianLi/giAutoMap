#pragma once
#include <iostream>
#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

//���ڷֱ���
enum RectMode
{
	FW_UNDIFINDE = -1,
	FW_MINIMIZE = 0,
	//FW_1920x1080,
	
	FW_5120x2880, // 16/9,		
	FW_3840x2160, // 16/9,		
	FW_3840x1620, // 64/27,		
	FW_3200x1800, // 16/9, 		
	FW_2560x1440, // 16/9, 		
	FW_2048x1536, // 4/3, 		
	FW_1920x2160, // 8/9, 		
	FW_1920x1200, // 8/5, 		
	FW_1920x1080, // 16/9, 		
	FW_1680x1050, // 8/5, 		
	FW_1600x1200, // 4/3, 		
	FW_1600x1024, // 25/16,		
	FW_1600x900,  // 16/9, 		
	FW_1400x1050, // 4/3,		
	FW_1440x900,  // 8/5, 		
	FW_1366x768,  // 683/384,	
	FW_1360x768,  // 85/48, 	
	FW_1280x1024, // 5/4, 		
	FW_1280x960,  // 4/3, 		
	FW_1280x800,  // 8/5, 		
	FW_1280x768,  // 5/3, 		
	FW_1152x864,  // 4/3,		
	FW_1176x664,  // 147/83,	
	FW_1024x768,  // 4/3, 		
	FW_1024x600,  // 128/75, 	
	FW_800x600,	  // 4/3,			
	

};

class ATM_GiState
{
public:
	//ԭ����Ϸ���ھ��
	HWND giHandle;
	//ԭ�񴰿�����
	RECT giRect;
	//ԭ�񴰿����򱣴�Ա���
	RECT giRectTmp;
	RECT giClientRect;

	//ԭ����Ϸ���ڽ�ͼ
	cv::Mat giFrame;
	//ԭ����Ϸ���ڽ�ͼ��Ч����
	//Mat giFrameROI;
	//
	cv::Mat giFrameRect;
	//
	cv::Mat giFramePaimon;
	//
	cv::Mat giFrameMap;

	cv::Mat giFrameUID;

	std::string giWndClass = "UnityWndClass";
	std::string giName = "ԭ��";

	//ԭ���Ƿ�������
	bool isRunning = false;
	//�����Ƿ�ɼ����Ƿ���������
	bool isPaimonVisible = false;
	//ԭ���Ƿ�ȫ��
	bool isFullScreen = false;
	//���ڷֱ���ģʽ
	int giRectMode = RectMode::FW_MINIMIZE;

	int resIdPaimon = 0;
	//���ڴ�С
	cv::Size giSize;

	bool isAutoScreen = true;

	bool isRun();
	void getHandle();
	void getRect();
	int getGiRectMode();
	int getGiState();
	cv::Point getOffset();

	void getAllScreen();

	//void getScreen(HWND handle); 
	void reSizeFrame();

	void getGiScreen();
	void getGiScreen2();
	void getGiFrame();
	void getGiFramePaimon();
	void getGiFrameMap();
	void getGiFrameUID();

	void setGiNameClass(LANGID SystemLanguageID);

	void setGiHandle(HWND GiHandle);
};

