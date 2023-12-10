#pragma once
#include <functional>
#include <opencv2/opencv.hpp>
#include "ATM_TM_ThreadBase.h"
#include "ATM_TM_Base_TemplateMatch.h"

class ATM_TM_TemplatePaimon
{
	cv::Mat _paimonTemplate;
	bool isPaimonVisible = false;

	ATM_TM_Base_TemplateMatch paimonTM;

public:
	cv::Mat _paimonMat;
	void setPaimonTemplate(cv::Mat paimonTemplateMat);
	void setPaimonMat(cv::Mat paimonMat);
	void TemplatePaimon();
	bool getPaimonVisible();
	bool getPaimonVisible(bool nullParm);
	bool getPaimonVisible(cv::Mat paimonMat);
};

class ATM_TM_TemplatePaimonT
{
	ATM_TM_TemplatePaimon paimon;
	ATM_TM_ThreadBase<bool, cv::Mat, ATM_TM_TemplatePaimon> tObj;
public:
	ATM_TM_TemplatePaimonT()
	{
		tObj.setFunction(std::bind((bool(ATM_TM_TemplatePaimon::*)(cv::Mat))&ATM_TM_TemplatePaimon::getPaimonVisible, &paimon,paimon._paimonMat));
	}

	bool setPaimonMat(cv::Mat paimonMat)
	{

		tObj.start();
	}

	bool getPaimonVisible(bool &isVisible)
	{
		return tObj.getWorkOut(isVisible);
	}

};