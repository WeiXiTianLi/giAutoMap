#pragma once
#include <iostream>
#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
using namespace std;
using namespace cv;

//基于分辨率
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
	//原神游戏窗口句柄
	HWND giHandle;
	//原神窗口区域
	RECT giRect;
	//原神窗口区域保存对比用
	RECT giRectTmp;
	RECT giClientRect;

	//原神游戏窗口截图
	Mat giFrame;
	//原神游戏窗口截图有效区域
	//Mat giFrameROI;
	//
	Mat giFrameRect;
	//
	Mat giFramePaimon;
	//
	Mat giFrameMap;

	Mat giFrameUID;

	string giWndClass = "UnityWndClass";
	string giName = "原神";

	//原神是否运行中
	bool isRunning = false;
	//派蒙是否可见即是否处于主画面
	bool isPaimonVisible = false;
	//原神是否全屏
	bool isFullScreen = false;
	//窗口分辨率模式
	int giRectMode = RectMode::FW_MINIMIZE;

	int resIdPaimon = 0;
	//窗口大小
	Size giSize;

	bool isAutoScreen = true;

	bool isRun();
	void getHandle();
	void getRect();
	int getGiRectMode();
	int getGiState();
	Point getOffset();

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

