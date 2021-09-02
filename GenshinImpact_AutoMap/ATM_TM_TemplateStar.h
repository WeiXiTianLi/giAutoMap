#pragma once
//#include <thread>
//#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

//#include <opencv2/imgproc/imgproc_c.h>
using namespace std;
using namespace cv;

class ATM_TM_TemplateStar
{
	Mat _starTemplate;
	Mat _starMat;
	bool isStarVisible = false;
	vector<Point2d> pos;
public:
	bool isInit = false;

	void Init();
	void setStarTemplate(Mat starTemplateMat);
	void setStarMat(Mat starMat);
	void TemplateStar();
	bool getStar();
	vector<Point2d> getStarPos();
};


