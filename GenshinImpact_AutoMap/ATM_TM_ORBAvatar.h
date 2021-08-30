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

class ATM_TM_ORBAvatar
{

	Mat _avatarTemplate;
	Mat _avatarMat;
	double rotationAngle = 0;

	Mat gray0;
	Mat gray1;
	Mat gray2;
	Mat and12;
	Mat dilate_element = getStructuringElement(MORPH_RECT, Size(2, 2));
	Mat erode_element = getStructuringElement(MORPH_RECT, Size(2, 2));

public:
	bool isInit = false;

	void setAvatarTemplate(Mat avatarTemplateMat);
	void setAvatarMat(Mat avatarMat);
	void Init();
	void ORBMatch();
	double getRotationAngle();
private:
	double dis(Point p);
	std::vector<Point2f> Vector2UnitVector(std::vector<Point2f> pLis);
	double Line2Angle(Point2f p);
};
