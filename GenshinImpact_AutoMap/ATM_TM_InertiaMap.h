#pragma once
#include <thread>
#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
using namespace std;
using namespace cv;

class ATM_TM_InertiaMap
{

	Mat _nowMapMat;
	Mat _beforeMapMat;

	int minHessian = 400;
	double ratio_thresh = 0.66;

	Ptr<xfeatures2d::SURF> detector;
	std::vector<KeyPoint> Kp_NowMap, Kp_BeforeMap;
	Mat Dp_NowMap, Dp_BeforeMap;

	Point2d pos;

public:
	//ATM_TM_InertiaMap();
	//~ATM_TM_InertiaMap();

public:
	bool isDealtWith = false;
	bool isContinuity = false;
	void setNowMap(Mat mapMat);

	void SURFMatch();
	//Point2d SURFMatch(Mat minMapMat);
	Point2d getPosOffset();
	bool getIsContinuity();
private:
	void swapMat();
	//double dis(Point2d &p);
	Point2d SPC(vector<double> lisx, double sumx, vector<double> lisy, double sumy);
	//Point SPC2(vector<double> lisx, double sumx, vector<double> lisy, double sumy);
	//double var(vector<double> lisx, double sumx, vector<double> lisy, double sumy);
};

