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

class ATM_TM_ORBAvatar
{

	cv::Mat _avatarTemplate;
	cv::Mat _avatarMat;
	double rotationAngle = 0;

	cv::Mat gray0;
	cv::Mat gray1;
	cv::Mat gray2;
	cv::Mat and12;
	cv::Mat dilate_element = getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
	cv::Mat erode_element = getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));

public:
	bool isInit = false;

	void setAvatarTemplate(cv::Mat avatarTemplateMat);
	void setAvatarMat(cv::Mat avatarMat);
	void Init();
	void ORBMatch();
	double getRotationAngle();
private:
	double dis(cv::Point p);
	std::vector<cv::Point2f> Vector2UnitVector(std::vector<cv::Point2f> pLis);
	double Line2Angle(cv::Point2f p);
};
