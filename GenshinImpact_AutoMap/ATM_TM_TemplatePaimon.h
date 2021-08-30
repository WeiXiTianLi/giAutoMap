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

class ATM_TM_TemplatePaimon
{
	Mat _paimonTemplate;
	Mat _paimonMat;
	bool isPaimonVisible = false;
public:
	void setPaimonTemplate(Mat paimonTemplateMat);
	void setPaimonMat(Mat paimonMat);
	void TemplatePaimon();
	bool getPaimonVisible();

};
