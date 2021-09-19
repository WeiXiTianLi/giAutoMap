#pragma once
#include <opencv2/opencv.hpp>
// 匹配 基类
template<class FunOut>
class ATM_TM_Base_Match
{
	//左图为匹配对象，目标所在图像
	cv::Mat _leftMat;
	//右图为被匹配对象，包含目标图像的图像
	cv::Mat _rightMat;

public:
	void setLeftMat(cv::Mat leftMat)
	{
		_leftMat = leftMat;
	}
	void setRightMat(cv::Mat rightMat)
	{
		_rightMat = rightMat;
	}

	//匹配
	virtual FunOut Match()=0;
	//virtual void* Match()=0;
};
