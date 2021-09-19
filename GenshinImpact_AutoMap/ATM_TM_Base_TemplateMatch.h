#pragma once
//#include <opencv2/opencv.hpp>
#include "ATM_TM_Base_Match.h"
using namespace cv;

//Ä£°åÆ¥Åä»ùÀà
class ATM_TM_Base_TemplateMatch : public ATM_TM_Base_Match<cv::Point2d>
//class ATM_TM_Base_TemplateMatch: public ATM_TM_Base_Match
{
	Mat _leftMat;
	Mat _rightMat;
	Mat tmp;
public:
	double minVal;
	double maxVal;
	Point minLoc;
	Point maxLoc;

	void setLeftMat(Mat leftMat);
	void setRightMat(Mat rightMat);

	bool Match(int method, InputArray mask = noArray());
	cv::Point2d Match() {



		return cv::Point2d();
	}
};

