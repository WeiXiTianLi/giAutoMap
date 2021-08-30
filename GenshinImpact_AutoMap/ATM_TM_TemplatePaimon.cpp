#include "ATM_TM_TemplatePaimon.h"

void ATM_TM_TemplatePaimon::setPaimonTemplate(Mat paimonTemplateMat)
{
	_paimonTemplate = paimonTemplateMat;
}

void ATM_TM_TemplatePaimon::setPaimonMat(Mat paimonMat)
{
	_paimonMat = paimonMat;
}

void ATM_TM_TemplatePaimon::TemplatePaimon()
{
	Mat tmp;
	matchTemplate(_paimonTemplate, _paimonMat, tmp, TM_CCOEFF_NORMED);

	double minVal, maxVal;
	Point minLoc, maxLoc;
	//—∞’“◊Óº—∆•≈‰Œª÷√
	minMaxLoc(tmp, &minVal, &maxVal, &minLoc, &maxLoc);
#ifdef _DEBUG
	cout << "Match Template MinVal & MaxVal" << minVal << " , " << maxVal << endl;
#endif
	if (minVal < 0.51 || maxVal == 1)
	{
		isPaimonVisible = false;
	}
	else
	{
		isPaimonVisible = true;
	}
}

bool ATM_TM_TemplatePaimon::getPaimonVisible()
{
	return isPaimonVisible;
}
