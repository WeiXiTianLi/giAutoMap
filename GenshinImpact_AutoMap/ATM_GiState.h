#pragma once
#include <iostream>
#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
using namespace std;
using namespace cv;

//基于比例
enum RectRatio
{
	RR_1_1,

	RR_16_9,

};
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

	/*
	16/9,
	16/9,
	64/27,
	16/9, 
	16/9, 
	4/3, 
	8/9, 
	8/5, 
	16/9, 
	8/5, 
	4/3, 
	25/16,
	16/9, 
	4/3,
	8/5, 
	683/384, 
	85/48, 
	5/4, 
	4/3, 
	8/5, 
	5/3, 
	4/3,
	147/83,
	4/3, 
	128/75, 
	4/3,
	4/3

*/
	//FW_2560x1050,
	//FW_2560x1080,
	//FW_2560x1440,
	//FW_3840x1440,
	//FW_3840x2160,
	//FW_5120x2880,
	//FW_2560x1080,

};

/*
5120*2880
3840*2160（超高清4k）
2560*1440
1920*1080（1080p全高清）
1600*900
1280*720（720p高清）
目前市场上的液晶显示器屏幕的长宽主要有四种比例，5:4、4:3（较少）、16:10、16:9（新品）。
5:4及4:3比例的20（20.1）英寸液晶显示器的最佳分辨率是：1600×1200。
5:4及4:3比例的19、17英寸液晶显示器的最佳分辨率是：1280×1024。
5:4及4:3比例的15、14英寸液晶显示器的最佳分辨率是：1024×768。
16:10比例的30英寸宽屏液晶显示器的最佳分辨率是：2560×1600。
16:10比例的28、27与26（25.5）、24英寸宽屏液晶显示器的最佳分辨率是：1920×1200。
16:10比例的22（21.6）与20（20.1）英寸宽屏液晶显示器的最佳分辨率是：1680×1050。
16:10比例的19（18.5）与17英寸宽屏液晶显示器的最佳分辨率是：1440×900。
16:10比例的14、13、12（12.1）英寸宽屏液晶显示器（多为笔记本电脑屏幕）的最佳分辨率是：1280×800。
16:9比例的46、25.5与24英寸宽屏液晶显示器的最佳分辨率是：1920×1200。
16:9比例的23与22（21.5）英寸宽屏液晶显示器的最佳分辨率是：1920×1080（1080p FullHD全高清分辨率）。
16:9比例的32与26英寸宽屏液晶电视的最佳分辨率是：1366×768。
16:9比例的19（18.5）与16（15.6）英寸宽屏液晶显示器的最佳分辨率是：1366×768。
16:9比例的15英寸宽屏液晶显示器的最佳分辨率是：1280×720。

*/
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

	void getGiScreen();
	void getGiScreen2();
	void getGiFrame();
	void getGiFramePaimon();
	void getGiFrameMap();
	void getGiFrameUID();

	void setGiNameClass(LANGID SystemLanguageID);

	void setGiHandle(HWND GiHandle);
};

