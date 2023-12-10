#pragma once
#include <thread>
#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/video/tracking.hpp>

#include <opencv2/imgproc/imgproc_c.h>

class ATM_TM_SurfMap
{
	cv::Mat _mapMat;
	cv::Mat _minMapMat;
	cv::Mat _minMapLastMat;

	int minHessian = 400;
	double ratio_thresh = 0.66;
	double mapScale = 1.33334;//1.3;
	int someSizeR = 106;
	double MatchMatScale = 2.0;

	cv::Ptr<cv::xfeatures2d::SURF> detector, detectorSomeMap;
	std::vector<cv::KeyPoint> Kp_MinMap, Kp_Map, Kp_SomeMap;
	cv::Mat Dp_MinMap, Dp_Map, Dp_SomeMap;

	//static Point hisP[3];
	cv::Point2d hisP[3];

	cv::Point2d pos;

	int stateNum = 2;
	int measureNum = 2;
	int controlNum = 2;

	cv::KalmanFilter KF;
	cv::Mat state; /* (phi, delta_phi) */
	cv::Mat processNoise;
	cv::Mat measurement;

	// ab's inertia filter
	cv::Mat last_mini_map;
	bool inited = false;
	bool orb_match(cv::Mat &img1, cv::Mat &img2, cv::Point2f &offset);
	void set_mini_map(const cv::Mat &giMiniMapRef);
	bool control_odometer_calculation(const cv::Mat &giMiniMapRef, cv::Point2d &control, double scale);
public:
	ATM_TM_SurfMap();
	~ATM_TM_SurfMap();

public:
	bool isInit = false;
	bool isContinuity = false;
	bool isConveying = false;
	bool isOnCity = false;
	void setMap(cv::Mat mapMat);
	void setMinMap(cv::Mat minMapMat);

	void Init();
	void SURFMatch();
	cv::Point2d SURFMatch(cv::Mat minMapMat);
	cv::Point2d getLocalPos();
	bool getIsContinuity();
private:
	double dis(cv::Point2d &p);
	cv::Point2d SPC(std::vector<double> lisx, double sumx, std::vector<double> lisy, double sumy);
	//Point SPC2(vector<double> lisx, double sumx, vector<double> lisy, double sumy);
	double var(std::vector<double> lisx, double sumx, std::vector<double> lisy, double sumy);
};
