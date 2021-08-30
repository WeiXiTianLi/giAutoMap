#pragma once
#include <thread>
#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/xfeatures2d/cuda.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/core/cuda.hpp>

#include <opencv2/imgproc/imgproc_c.h>
using namespace std;
using namespace cv;

class ATM_TM_ThreadBase
{
	//实现线程的单独控制

	thread *tLoopWork = nullptr;
	void(*ptr)(Mat &inMat) = nullptr;
	bool isExitThread = false;
	bool isExistFunction = false;
	bool isRunWork = false;
	bool isEndWork = false;

	void run();

public:
	bool isInit = false;
	ATM_TM_ThreadBase();
	~ATM_TM_ThreadBase();
	ATM_TM_ThreadBase(void(*funPtr)(Mat &inMat));

	void setFunction(void(*funPtr)(Mat &inMat));
	virtual void workfun();
	void start(Mat & inMat);
	bool isEnd();
};