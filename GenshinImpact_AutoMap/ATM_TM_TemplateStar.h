#pragma once
#include <opencv2/opencv.hpp>

class ATM_TM_TemplateStar
{
	cv::Mat _starTemplate;
	cv::Mat _starMat;
	bool isStarVisible = false;
	std::vector<cv::Point2d> pos;
public:
	bool isInit = false;

	void Init();
	void setStarTemplate(cv::Mat starTemplateMat);
	void setStarMat(cv::Mat starMat);
	void TemplateStar();
	bool getStar();
	std::vector<cv::Point2d> getStarPos();
};


