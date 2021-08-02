#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;
namespace ATM_Modules
{
	double dis(double x1,double x2);
	double dis(Point2d p);
	double dis(Point2d p1, Point2d p2);

	bool isContains(Rect & r, Point2d & p);
};
