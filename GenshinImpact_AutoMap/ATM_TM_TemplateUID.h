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

class ATM_TM_TemplateUID
{
	struct ATM_TM_Template_NumUID
	{
		Mat UID;
		Mat n[10];
		int max = 10;
	}giNumUID;

	Mat _uidMat;

	int _NumBit[9] = { 0 };
	int _uid = 0;

	int getMaxID(double lis[], int len);

public:
	bool isInit = false;

	void Init();
	void setUIDTemplate(Mat *uidTemplateMat);
	void setUIDMat(Mat uidMat);
	void TemplateUID();
	int getUID();
};