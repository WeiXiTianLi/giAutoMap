#pragma once
#include <thread>
#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

class ATM_TM_InertiaMap
{

	cv::Mat _nowMapMat;
	cv::Mat _beforeMapMat;

	int minHessian = 400;
	double ratio_thresh = 0.66;

	cv::Ptr<cv::xfeatures2d::SURF> detector;
	std::vector<cv::KeyPoint> Kp_NowMap, Kp_BeforeMap;
	cv::Mat Dp_NowMap, Dp_BeforeMap;

	cv::Point2d pos;

public:
	//ATM_TM_InertiaMap();
	//~ATM_TM_InertiaMap();

public:
	bool isDealtWith = false;
	bool isContinuity = false;
	void setNowMap(cv::Mat mapMat);

	void SURFMatch();
	//Point2d SURFMatch(Mat minMapMat);
	cv::Point2d getPosOffset();
	bool getIsContinuity();
private:
	void swapMat();
	//double dis(Point2d &p);
	cv::Point2d SPC(std::vector<double> lisx, double sumx, std::vector<double> lisy, double sumy);
	//Point SPC2(vector<double> lisx, double sumx, vector<double> lisy, double sumy);
	//double var(vector<double> lisx, double sumx, vector<double> lisy, double sumy);
};

