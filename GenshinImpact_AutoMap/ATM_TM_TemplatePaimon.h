#pragma once
#include <functional>
#include <opencv2/opencv.hpp>
#include "ATM_TM_ThreadBase.h"
#include "ATM_TM_Base_TemplateMatch.h"
using namespace std;
using namespace cv;

class ATM_TM_TemplatePaimon
{
	Mat _paimonTemplate;
	bool isPaimonVisible = false;

	ATM_TM_Base_TemplateMatch paimonTM;

public:
	Mat _paimonMat;
	void setPaimonTemplate(Mat paimonTemplateMat);
	void setPaimonMat(Mat paimonMat);
	void TemplatePaimon();
	bool getPaimonVisible();
	bool getPaimonVisible(bool nullParm);
	bool getPaimonVisible(Mat paimonMat);
};

class ATM_TM_TemplatePaimonT
{
	ATM_TM_TemplatePaimon paimon;
	ATM_TM_ThreadBase<bool, Mat, ATM_TM_TemplatePaimon> tObj;
public:
	ATM_TM_TemplatePaimonT()
	{
		tObj.setFunction(bind((bool(ATM_TM_TemplatePaimon::*)(Mat))&ATM_TM_TemplatePaimon::getPaimonVisible, &paimon,paimon._paimonMat));
	}

	bool setPaimonMat(Mat paimonMat)
	{

		tObj.start();
	}

	bool getPaimonVisible(bool &isVisible)
	{
		return tObj.getWorkOut(isVisible);
	}

};