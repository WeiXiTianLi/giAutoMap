#include "ATM_TM_TemplateStar.h"

void ATM_TM_TemplateStar::Init()
{
	if (isInit)return;

	isInit = true;
}

void ATM_TM_TemplateStar::setStarTemplate(cv::Mat starTemplateMat)
{
	_starTemplate = starTemplateMat;
}

void ATM_TM_TemplateStar::setStarMat(cv::Mat starMat)
{
	_starMat = starMat;
}

void ATM_TM_TemplateStar::TemplateStar()
{
	int MAXLOOP = 0;
	bool isLoopMatch = false;
	cv::Mat tmp;
	double minVal, maxVal;
	cv::Point minLoc, maxLoc;

	pos.clear();

	matchTemplate(_starTemplate, _starMat, tmp, cv::TM_CCOEFF_NORMED);
	minMaxLoc(tmp, &minVal, &maxVal, &minLoc, &maxLoc);
#ifdef _DEBUG
	cout << "Match Star MinVal & MaxVal : " << minVal << " , " << maxVal << endl;
#endif
	if (maxVal < 0.66)
	{
		isStarVisible = false;
	}
	else
	{
		isLoopMatch = true;
		isStarVisible = true;
		pos.push_back(maxLoc - cv::Point(_starMat.cols / 2, _starMat.rows / 2) + cv::Point(_starTemplate.cols / 2, _starTemplate.rows / 2));
	}

	while (isLoopMatch)
	{
		_starMat(cv::Rect(maxLoc.x, maxLoc.y, _starTemplate.cols, _starTemplate.rows)) = cv::Scalar(0, 0, 0);
		matchTemplate(_starTemplate, _starMat, tmp, cv::TM_CCOEFF_NORMED);
		minMaxLoc(tmp, &minVal, &maxVal, &minLoc, &maxLoc);
#ifdef _DEBUG
		cout << "Match Star MinVal & MaxVal : " << minVal << " , " << maxVal << endl;
#endif
		if (maxVal < 0.66)
		{
			isLoopMatch = false;
		}
		else
		{
			pos.push_back(maxLoc - cv::Point(_starMat.cols / 2, _starMat.rows / 2) + cv::Point(_starTemplate.cols / 2, _starTemplate.rows / 2));
		}

		MAXLOOP > 10 ? isLoopMatch = false : MAXLOOP++;
	}
}

bool ATM_TM_TemplateStar::getStar()
{
	return isStarVisible;
}

std::vector<cv::Point2d> ATM_TM_TemplateStar::getStarPos()
{
	return pos;
}