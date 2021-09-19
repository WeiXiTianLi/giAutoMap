#include "ATM_TM_InertiaMap.h"

void ATM_TM_InertiaMap::setNowMap(Mat mapMat)
{
	_nowMapMat = mapMat;
	isDealtWith = false;
}


void ATM_TM_InertiaMap::SURFMatch()
{
	//图片未更新
	if (isDealtWith)
	{
		return;
	}
	//图片为空
	if (_nowMapMat.empty() || _beforeMapMat.empty())
	{
		return;
	}

	Mat nowMap(_nowMapMat);
	Mat beforeMap(_beforeMapMat);

	//偏移归零
	pos = Point2d(0, 0);

	//匹配偏移
	detector = xfeatures2d::SURF::create(minHessian);
	detector->detectAndCompute(nowMap, noArray(), Kp_NowMap, Dp_NowMap);
	detector->detectAndCompute(beforeMap, noArray(), Kp_BeforeMap, Dp_BeforeMap);
	if (Kp_NowMap.size() == 0 || Kp_BeforeMap.size() == 0)
	{
		//不连续
		isContinuity = false;
	}
	else
	{
		Ptr<DescriptorMatcher> matcherTmp = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
		std::vector< std::vector<DMatch> > KNN_mTmp;
#ifdef _DEBUG
		std::vector<DMatch> good_matchesTmp;
#endif
		matcherTmp->knnMatch(Dp_NowMap, Dp_BeforeMap, KNN_mTmp, 2);
		std::vector<double> lisx;
		std::vector<double> lisy;
		double sumx = 0;
		double sumy = 0;
		for (size_t i = 0; i < KNN_mTmp.size(); i++)
		{
			if (KNN_mTmp[i][0].distance < ratio_thresh * KNN_mTmp[i][1].distance)
			{
#ifdef _DEBUG
				good_matchesTmp.push_back(KNN_mTmp[i][0]);
#endif
				// 这里有个bug回卡进来，进入副本或者切换放大招时偶尔触发
				if (KNN_mTmp[i][0].queryIdx >= Kp_NowMap.size())
				{
					continue;
				}

				lisx.push_back(((nowMap.cols / 2.0 - Kp_NowMap[KNN_mTmp[i][0].queryIdx].pt.x) + Kp_BeforeMap[KNN_mTmp[i][0].trainIdx].pt.x));
				lisy.push_back(((nowMap.rows / 2.0 - Kp_NowMap[KNN_mTmp[i][0].queryIdx].pt.y) + Kp_BeforeMap[KNN_mTmp[i][0].trainIdx].pt.y));
				sumx += lisx.back();
				sumy += lisy.back();
			}
		}
#ifdef _DEBUG
		Mat img_matches, imgmap, imgnowMap;
		drawKeypoints(beforeMap, Kp_BeforeMap, imgmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		drawKeypoints(nowMap, Kp_NowMap, imgnowMap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		drawMatches(beforeMap, Kp_NowMap, nowMap, Kp_BeforeMap, good_matchesTmp, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
#endif
		//好的特征点对的数量过少也结束
		if (min(lisx.size(), lisy.size()) == 0)
		{

		}
		else
		{
			pos = SPC(lisx, sumx, lisy, sumy);
			isContinuity = true;
		}
	}



	//匹配正常结束后将图交换并标记为已处理
	isDealtWith = true;
	swapMat();
}

//返回坐标(偏移
Point2d ATM_TM_InertiaMap::getPosOffset()
{
	return pos;
}

//返回连续性
bool ATM_TM_InertiaMap::getIsContinuity()
{
	return isContinuity;
}

void ATM_TM_InertiaMap::swapMat()
{
	_beforeMapMat = _nowMapMat;
}


Point2d ATM_TM_InertiaMap::SPC(vector<double> lisx, double sumx, vector<double> lisy, double sumy)
{
	//这个剔除异常点算法
	//回头要改
	Point2d mpos;
	double meanx = sumx / lisx.size(); //均值
	double meany = sumy / lisy.size(); //均值
	double x = meanx;
	double y = meany;
	if (min(lisx.size(), lisy.size()) > 3)
	{
		double accumx = 0.0;
		double accumy = 0.0;
		for (int i = 0; i < min(lisx.size(), lisy.size()); i++)
		{
			accumx += (lisx[i] - meanx)*(lisx[i] - meanx);
			accumy += (lisy[i] - meany)*(lisy[i] - meany);
		}

		double stdevx = sqrt(accumx / (lisx.size() - 1)); //标准差
		double stdevy = sqrt(accumy / (lisy.size() - 1)); //标准差

		sumx = 0;
		sumy = 0;
		double numx = 0;
		double numy = 0;
		for (int i = 0; i < min(lisx.size(), lisy.size()); i++)
		{
			if (abs(lisx[i] - meanx) < 1 * stdevx)
			{
				sumx += lisx[i];
				numx++;
			}

			if (abs(lisy[i] - meany) < 1 * stdevy)
			{
				sumy += lisy[i];
				numy++;
			}
		}
		double xx = sumx / numx;
		double yy = sumy / numy;
		mpos = Point2d(xx, yy);
	}
	else
	{
		mpos = Point2d(x, y);
	}
	return mpos;
}
