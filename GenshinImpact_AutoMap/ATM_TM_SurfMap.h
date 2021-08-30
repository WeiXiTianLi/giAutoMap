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
#include <opencv2/video/tracking.hpp>

#include <opencv2/imgproc/imgproc_c.h>
using namespace std;
using namespace cv;

class ATM_TM_SurfMap
{
	Mat _mapMat;
	Mat _minMapMat;
	Mat _minMapLastMat;

	int minHessian = 400;
	double ratio_thresh = 0.66;
	double mapScale = 1.3;//1.3;
	int someSizeR = 106;
	double MatchMatScale = 2.0;

	Ptr<xfeatures2d::SURF> detector, detectorSomeMap;
	std::vector<KeyPoint> Kp_MinMap, Kp_Map, Kp_SomeMap;
	Mat Dp_MinMap, Dp_Map, Dp_SomeMap;

	//static Point hisP[3];
	Point2d hisP[3];

	Point2d pos;

	 int stateNum = 4;
	 int measureNum = 2;

	KalmanFilter KF;
	Mat state; /* (phi, delta_phi) */
	Mat processNoise;
	Mat measurement;

public:
	ATM_TM_SurfMap();
	~ATM_TM_SurfMap();

public:
	bool isInit = false;
	bool isContinuity = false;
	bool isOnCity = false;
	void setMap(Mat mapMat);
	void setMinMap(Mat minMapMat);

	void Init();
	void SURFMatch();
	Point2d SURFMatch(Mat minMapMat);
	Point2d getLocalPos();
	bool getIsContinuity();
private:
	double dis(Point2d &p);
	Point2d SPC(vector<double> lisx, double sumx, vector<double> lisy, double sumy);
	//Point SPC2(vector<double> lisx, double sumx, vector<double> lisy, double sumy);
	double var(vector<double> lisx, double sumx, vector<double> lisy, double sumy);
};
