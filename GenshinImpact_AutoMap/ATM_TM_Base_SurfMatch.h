#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/video/tracking.hpp>
using namespace cv;

// SURF匹配 基类
class ATM_TM_Base_SurfMatch
{
	//左图为匹配对象，目标所在图像
	Mat _leftMat;
	//右图为被匹配对象，包含目标图像的图像
	Mat _rightMat;

public:
	//void setLeftMat(Mat leftMat);
	//void setRightMat(Mat rightMat);

	////匹配
	//bool Match();
};

//class ATM_TM_Base_SurfMatch2 :private ATM_TM_Base_Match<bool>
//{
//
//public:
//	void setLeftMat(Mat leftMat);
//	void setRightMat(Mat rightMat);
//
//	bool Match();
//};