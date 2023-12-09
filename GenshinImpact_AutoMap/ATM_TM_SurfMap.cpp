#include "ATM_TM_SurfMap.h"

bool ATM_TM_SurfMap::orb_match(cv::Mat &img1, cv::Mat &img2, cv::Point2f &offset) 
{
	auto beg_time = std::chrono::steady_clock::now();
    
    auto img1_cp = img1.clone();
    auto img2_cp = img2.clone();
    // resize 是等比例缩放，就不超了

    // 分别计算orb特征点
    cv::Ptr<cv::ORB> orb = cv::ORB::create(5000);
    std::vector<cv::KeyPoint> kp1, kp2;
    cv::Mat desp1, desp2;
    orb->detectAndCompute(img1_cp, cv::Mat(), kp1, desp1);
    orb->detectAndCompute(img2_cp, cv::Mat(), kp2, desp2);
    
    if (desp1.empty() || desp2.empty()) {
        return false;
    }
    
    // 首先采用knnMatch剔除最近匹配点距离与次近匹配点距离比率大于0.6的舍去
    cv::BFMatcher matcher(cv::NORM_HAMMING);
    std::vector<std::vector<cv::DMatch>> matches;
    matcher.knnMatch(desp1, desp2, matches, 2);
    // 解决没有匹配点的情况
    if (matches.size() == 0) {
        return false;
    }
    std::vector<cv::DMatch> good_matches;
    for (int i = 0; i < matches.size(); i++) {
        if (matches[i][0].distance < 0.6 * matches[i][1].distance) {
            good_matches.push_back(matches[i][0]);
        }
    }

    if (good_matches.size() == 0) {
        return false;
    }

    // auto img2_copy = img2_cp.clone();
    // 画出good matches，然后保存
    // cv::drawMatches(img1_cp, kp1, img2_cp, kp2, good_matches, img2_copy);
    // cv::imwrite("good_matches.jpg", img2_copy);

    // 计算偏移量，直接取平均
    cv::Point2f sum_offset(0, 0);
    for (int i = 0; i < good_matches.size(); i++) {
        sum_offset += kp2[good_matches[i].trainIdx].pt - kp1[good_matches[i].queryIdx].pt;
    }
    offset = cv::Point2f(sum_offset.x / good_matches.size(), sum_offset.y / good_matches.size());
    auto end_time = std::chrono::steady_clock::now();
    auto time_cost = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - beg_time).count();
    std::cout << "orb match time cost: " << time_cost << " ms" << std::endl;
    return true;

}

void ATM_TM_SurfMap::set_mini_map(const cv::Mat &giMiniMapRef) 
{
    last_mini_map = giMiniMapRef.clone();
    inited = true;
}

bool ATM_TM_SurfMap::control_odometer_calculation(const cv::Mat &giMiniMapRef, cv::Point2d &control, double scale)
{
    if (!inited) {
        // JUST INIT IT
        last_mini_map = giMiniMapRef.clone();
        inited = true;
        control = cv::Point2d(0, 0);
        return false;
    }
    else {
        auto curr_mini_map = giMiniMapRef.clone();
        // use orb match to get the u, aka offset
        cv::Point2f offset;
        if (orb_match(last_mini_map, curr_mini_map, offset)) {
            control = cv::Point2d( -offset.x * scale, -offset.y * scale);
            last_mini_map = curr_mini_map.clone();
            return true;
        }
        else {
            control = cv::Point2d(0, 0);
            return false;
        }
    }
}


ATM_TM_SurfMap::ATM_TM_SurfMap()
{
	hisP[0] = Point();
	hisP[1] = Point();
	hisP[2] = Point();

	KF = KalmanFilter(stateNum, measureNum, controlNum);
	state = Mat(stateNum, 1, CV_32F); //state(x,y,detaX,detaY)
	processNoise= Mat(stateNum, 1, CV_32F);
	measurement = Mat::zeros(measureNum, 1, CV_32F);	//measurement(x,y)

	randn(state, Scalar::all(0), Scalar::all(0.1)); //随机生成一个矩阵，期望是0，标准差为0.1;
	// set A
	KF.transitionMatrix = (cv::Mat_<float>(stateNum, stateNum) <<
		1, 0, 
		0, 1);
	// set B
	KF.controlMatrix = (cv::Mat_<float>(stateNum, controlNum) <<
		1, 0,
		0, 1);
	// set Q
	setIdentity(KF.processNoiseCov, cv::Scalar::all(1e-5));
	// set H
	KF.measurementMatrix = (cv::Mat_<float>(measureNum, stateNum) <<
		1, 0,
		0, 1);
	// set R
	setIdentity(KF.measurementNoiseCov, cv::Scalar::all(1e-3));

	randn(KF.statePost, cv::Scalar::all(0), cv::Scalar::all(0.1));

}

ATM_TM_SurfMap::~ATM_TM_SurfMap()
{
}

void ATM_TM_SurfMap::setMap(Mat mapMat)
{
	_mapMat = mapMat;
}

void ATM_TM_SurfMap::setMinMap(Mat minMapMat)
{
	_minMapMat = minMapMat;
}

void ATM_TM_SurfMap::Init()
{
	if (isInit)return;
	detector = xfeatures2d::SURF::create(minHessian);
	detector->detectAndCompute(_mapMat, noArray(), Kp_Map, Dp_Map);
	isInit = true;
}

void ATM_TM_SurfMap::SURFMatch()
{
	//static Point hisP[3];

	Mat img_scene(_mapMat);
	Mat img_object(_minMapMat(Rect(30, 30, _minMapMat.cols - 60, _minMapMat.rows - 60)));

	//someSizeR = cvCeil(img_object.rows / 2);

	if (img_object.empty())
	{
		return;
	}

	isContinuity = false;
	Point2d dp1 = hisP[1] - hisP[0];
	Point2d dp2 = hisP[2] - hisP[1];

	//角色移动连续性判断
	if (dis(dp2) < 1000)
	{
		if (hisP[2].x > someSizeR && hisP[2].x < img_scene.cols - someSizeR && hisP[2].y>someSizeR && hisP[2].y < img_scene.rows - someSizeR)
		{
			isContinuity = true;
			if (isContinuity)
			{

				//				
				//				//惯导部分
				

				if (isOnCity == false)
				{
					//不在城镇中时
					Mat someMap(img_scene(Rect(static_cast<int>(hisP[2].x - someSizeR), static_cast<int>(hisP[2].y - someSizeR), someSizeR * 2, someSizeR * 2)));
					Mat minMap(img_object);
					//resize(someMap, someMap, Size(), MatchMatScale, MatchMatScale, 1);
					//resize(minMap, minMap, Size(), MatchMatScale, MatchMatScale, 1);

					detectorSomeMap = xfeatures2d::SURF::create(minHessian);
					detectorSomeMap->detectAndCompute(someMap, noArray(), Kp_SomeMap, Dp_SomeMap);
					detectorSomeMap->detectAndCompute(minMap, noArray(), Kp_MinMap, Dp_MinMap);
					if (Kp_SomeMap.size() == 0 || Kp_MinMap.size() == 0)
					{
						isContinuity = false;
					}
					else
					{
						Ptr<DescriptorMatcher> matcherTmp = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
						std::vector< std::vector<DMatch> > KNN_mTmp;
#ifdef _DEBUG
						std::vector<DMatch> good_matchesTmp;
#endif
						matcherTmp->knnMatch(Dp_MinMap, Dp_SomeMap, KNN_mTmp, 2);
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
								if (KNN_mTmp[i][0].queryIdx >= Kp_MinMap.size())
								{
									continue;
								}

								lisx.push_back(((minMap.cols / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.x)*mapScale + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.x));
								lisy.push_back(((minMap.rows / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.y)*mapScale + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.y));
								sumx += lisx.back();
								sumy += lisy.back();
							}
						}
#ifdef _DEBUG
						Mat img_matches, imgmap, imgminmap;
						drawKeypoints(someMap, Kp_SomeMap, imgmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
						drawKeypoints(img_object, Kp_MinMap, imgminmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

						drawMatches(img_object, Kp_MinMap, someMap, Kp_SomeMap, good_matchesTmp, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
#endif
#ifdef _DEBUG
						if (var(lisx, sumx, lisy, sumy) > 10)
						{

						}
#endif
						if (min(lisx.size(), lisy.size()) <= 4)
						{

							//有可能处于城镇中

							/***********************/
							//重新从完整中地图取出角色周围部分地图
							img_scene(Rect(hisP[2].x - someSizeR, hisP[2].y - someSizeR, someSizeR * 2, someSizeR * 2)).copyTo(someMap);
							//Mat minMap(img_object);

							resize(someMap, someMap, Size(someSizeR * 4, someSizeR * 4));
							//resize(minMap, minMap, Size(), MatchMatScale, MatchMatScale, 1);

							detectorSomeMap = xfeatures2d::SURF::create(minHessian);
							detectorSomeMap->detectAndCompute(someMap, noArray(), Kp_SomeMap, Dp_SomeMap);
							//detectorSomeMap->detectAndCompute(minMap, noArray(), Kp_MinMap, Dp_MinMap);
							if (Kp_SomeMap.size() == 0 || Kp_MinMap.size() == 0)
							{
								isContinuity = false;
							}
							else
							{
								Ptr<DescriptorMatcher> matcherTmp = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
								std::vector< std::vector<DMatch> > KNN_mTmp;
#ifdef _DEBUG
								std::vector<DMatch> good_matchesTmp;
#endif
								matcherTmp->knnMatch(Dp_MinMap, Dp_SomeMap, KNN_mTmp, 2);
								//std::vector<double> lisx;
								//std::vector<double> lisy;
								lisx.clear();
								lisy.clear();
								//double sumx = 0;
								//double sumy = 0;
								sumx = 0;
								sumy = 0;

								for (size_t i = 0; i < KNN_mTmp.size(); i++)
								{
									if (KNN_mTmp[i][0].distance < ratio_thresh * KNN_mTmp[i][1].distance)
									{
#ifdef _DEBUG
										good_matchesTmp.push_back(KNN_mTmp[i][0]);
#endif
										// 这里有个bug回卡进来，进入副本或者切换放大招时偶尔触发
										if (KNN_mTmp[i][0].queryIdx >= Kp_MinMap.size())
										{
											continue;
										}

										lisx.push_back(((minMap.cols / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.x)*0.8667 + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.x));
										lisy.push_back(((minMap.rows / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.y)*0.8667 + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.y));
										sumx += lisx.back();
										sumy += lisy.back();
									}
								}
#ifdef _DEBUG
								//Mat img_matches, imgmap, imgminmap;
								drawKeypoints(someMap, Kp_SomeMap, imgmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
								drawKeypoints(img_object, Kp_MinMap, imgminmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

								drawMatches(img_object, Kp_MinMap, someMap, Kp_SomeMap, good_matchesTmp, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
#endif
								if (min(lisx.size(), lisy.size()) <= 4)
								{
									isContinuity = false;
								}
								else
								{
									if (min(lisx.size(), lisy.size()) >= 10)
									{
										isOnCity = true;
									}
									else
									{
										isOnCity = false;
									}

									//double meanx = sumx / lisx.size(); //均值
									//double meany = sumy / lisy.size(); //均值
									Point2d p = SPC(lisx, sumx, lisy, sumy);

									//int x = (int)meanx;
									//int y = (int)meany;

									double x = (p.x - someMap.cols / 2.0) / 2.0;
									double y = (p.y - someMap.rows / 2.0) / 2.0;

									pos = Point2d(x + hisP[2].x, y + hisP[2].y);
								}
							}
							/***********************/
							//isContinuity = false;
						}
						else
						{
							isOnCity = false;

							//double meanx = sumx / lisx.size(); //均值
							//double meany = sumy / lisy.size(); //均值
							Point2d p = SPC(lisx, sumx, lisy, sumy);


							double x = p.x;
							double y = p.y;

							pos = Point2d(x + hisP[2].x - someSizeR, y + hisP[2].y - someSizeR);
						}
					}
				}
				else
				{
					//在城镇中
					/***********************/
					//重新从完整中地图取出角色周围部分地图
					Mat someMap(img_scene(Rect(hisP[2].x - someSizeR, hisP[2].y - someSizeR, someSizeR * 2, someSizeR * 2)));
					Mat minMap(img_object);

					resize(someMap, someMap, Size(someSizeR * 4, someSizeR * 4));
					//resize(minMap, minMap, Size(), MatchMatScale, MatchMatScale, 1);

					detectorSomeMap = xfeatures2d::SURF::create(minHessian);
					detectorSomeMap->detectAndCompute(someMap, noArray(), Kp_SomeMap, Dp_SomeMap);
					detectorSomeMap->detectAndCompute(minMap, noArray(), Kp_MinMap, Dp_MinMap);

					if (Kp_SomeMap.size() >= 2 && Kp_MinMap.size() >= 2)
					{
						Ptr<DescriptorMatcher> matcherTmp = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
						std::vector< std::vector<DMatch> > KNN_mTmp;
#ifdef _DEBUG
						std::vector<DMatch> good_matchesTmp;
#endif
						matcherTmp->knnMatch(Dp_MinMap, Dp_SomeMap, KNN_mTmp, 2);
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
								if (KNN_mTmp[i][0].queryIdx >= Kp_MinMap.size())
								{
									continue;
								}

								lisx.push_back(((minMap.cols / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.x)*0.8667 + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.x));
								lisy.push_back(((minMap.rows / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.y)*0.8667 + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.y));
								sumx += lisx.back();
								sumy += lisy.back();
							}
						}
#ifdef _DEBUG
						Mat img_matches, imgmap, imgminmap;
						drawKeypoints(someMap, Kp_SomeMap, imgmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
						drawKeypoints(img_object, Kp_MinMap, imgminmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

						drawMatches(img_object, Kp_MinMap, someMap, Kp_SomeMap, good_matchesTmp, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
#endif
						if (max(lisx.size(), lisy.size()) > 4)
						{
							if (min(lisx.size(), lisy.size()) >= 10)
							{
								isOnCity = true;
							}
							else
							{
								isOnCity = false;
							}
							//if(max(lisx.size(), lisy.size()) <=10 )
							//{
							//	isOnCity = false;
							//}

							//double meanx = sumx / lisx.size(); //均值
							//double meany = sumy / lisy.size(); //均值
							Point2d p = SPC(lisx, sumx, lisy, sumy);

							double x = (p.x - someMap.cols / 2.0) / 2.0;
							double y = (p.y - someMap.rows / 2.0) / 2.0;

							pos = Point2d(x + hisP[2].x, y + hisP[2].y);
						}
						else
						{
							isContinuity = false;
						}//if (max(lisx.size(), lisy.size()) > 4)
					}
					else
					{
						isContinuity = false;
					}//if (Kp_SomeMap.size() != 0 && Kp_MinMap.size() != 0)
				}// if(isOnCity==false)
			}//if (isContinuity)
		}
	}//if ((dis(dp1) + dis(dp2)) < 2000)
	else
	{
		isConveying = true;
	}
	if (!isContinuity)
	{
		detector->detectAndCompute(img_object, noArray(), Kp_MinMap, Dp_MinMap);

		if (Kp_MinMap.size() == 0)
		{
#ifdef _DEBUG
			cout << "SURF Match Fail Not KeyPoint" << endl;
#endif // _DEBUG
			return;
		}
		else
		{
			Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
			std::vector< std::vector<DMatch> > KNN_m;
#ifdef _DEBUG
			std::vector<DMatch> good_matches;
#endif
			matcher->knnMatch(Dp_MinMap, Dp_Map, KNN_m, 2);

			std::vector<double> lisx;
			std::vector<double> lisy;
			double sumx = 0;
			double sumy = 0;
			for (size_t i = 0; i < KNN_m.size(); i++)
			{
				if (KNN_m[i][0].distance < ratio_thresh * KNN_m[i][1].distance)
				{
#ifdef _DEBUG
					good_matches.push_back(KNN_m[i][0]);
#endif
					if (KNN_m[i][0].queryIdx >= Kp_MinMap.size())
					{
						continue;
					}
					lisx.push_back(((img_object.cols / 2.0 - Kp_MinMap[KNN_m[i][0].queryIdx].pt.x)*mapScale + Kp_Map[KNN_m[i][0].trainIdx].pt.x));
					lisy.push_back(((img_object.rows / 2.0 - Kp_MinMap[KNN_m[i][0].queryIdx].pt.y)*mapScale + Kp_Map[KNN_m[i][0].trainIdx].pt.y));
					sumx += lisx.back();
					sumy += lisy.back();
				}
			}

#ifdef _DEBUG
			Mat img_matches, imgmap, imgminmap;
			drawKeypoints(img_scene, Kp_Map, imgmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
			drawKeypoints(img_object, Kp_MinMap, imgminmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

			drawMatches(img_object, Kp_MinMap, img_scene, Kp_Map, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
#endif

			if (min(lisx.size(), lisy.size()) == 0)
			{
#ifdef _DEBUG
				cout << "SURF Match Fail" << endl;
#endif
				return;
			}
			else
			{
#ifdef _DEBUG
				cout << "SURF Match Point Number: " << lisx.size() << "," << lisy.size() << endl;

#endif

				pos = SPC(lisx, sumx, lisy, sumy);
			}
		}
	}
	auto pos_raw = cv::Point2d(pos.x, pos.y);
	auto u_k = cv::Point2d(0, 0);
	auto od_valid = control_odometer_calculation(img_object, u_k, 1);
	if (!od_valid)
	{
		cout << "no u_k update" << " with pos: " << pos << endl;
		// KF.init(stateNum, measureNum, 0);

		KF.statePost.at<float>(0) = pos.x;
		KF.statePost.at<float>(1) = pos.y;

		Mat prediction = KF.predict();
		Point2d predictPt = Point2d(prediction.at<float>(0), prediction.at<float>(1));

		//3.update measurement
		measurement.at<float>(0, 0) = static_cast<float>(pos.x);
		measurement.at<float>(1, 0) = static_cast<float>(pos.y);

		//4.update
		KF.correct(measurement);

		pos = Point2d(KF.statePost.at<float>(0), KF.statePost.at<float>(1));
		//isConveying = false;
		set_mini_map(img_object);
	}
	else
	{
		cout << "u_k: " << u_k << " with pos: " << pos << endl;
		// cv::imshow("img_object", img_object);
		// cv::waitKey();

		cv::Mat u_k_mat = cv::Mat::zeros(controlNum, 1, CV_32F);
		u_k_mat.at<float>(0, 0) = u_k.x;
		u_k_mat.at<float>(1, 0) = u_k.y;
		//Point statePt = Point((int)KF.statePost.at<float>(0), (int)KF.statePost.at<float>(1));

		//2.kalman prediction   
		Mat prediction = KF.predict(u_k_mat);
		Point2d predictPt = Point2d(prediction.at<float>(0), prediction.at<float>(1));

		//3.update measurement
		measurement.at<float>(0, 0) = static_cast<float>(pos.x);
		measurement.at<float>(1, 0) = static_cast<float>(pos.y);

		//4.update
		KF.correct(measurement);

		pos = Point2d(KF.statePost.at<float>(0), KF.statePost.at<float>(1));
	}
	// pos = pos_raw;

	hisP[0] = hisP[1];
	hisP[1] = hisP[2];
	hisP[2] = pos;

}

Point2d ATM_TM_SurfMap::SURFMatch(Mat minMapMat)
{
	return Point2d();
}

Point2d ATM_TM_SurfMap::getLocalPos()
{
	return pos;
}

bool ATM_TM_SurfMap::getIsContinuity()
{
	return isContinuity;
}

double ATM_TM_SurfMap::dis(Point2d & p)
{
	return sqrt(p.x*p.x + p.y*p.y);
}

Point2d ATM_TM_SurfMap::SPC(vector<double> lisx, double sumx, vector<double> lisy, double sumy)
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

//Point ATM_TM_SurfMap::SPC2(vector<double> lisx, double sumx, vector<double> lisy, double sumy)
//{
//	Point p;
//
//	return Point();
//}

double ATM_TM_SurfMap::var(vector<double> lisx, double sumx, vector<double> lisy, double sumy)
{
	double accumx = 0.0;
	double accumy = 0.0;
	for (int i = 0; i < min(lisx.size(), lisy.size()); i++)
	{
		accumx = (lisx[i] - sumx)*(lisx[i] - sumx);
		accumy = (lisy[i] - sumy)*(lisy[i] - sumy);
	}
	double stdevx = sqrt(accumx / (lisx.size() - 1));
	double stdevy = sqrt(accumy / (lisy.size() - 1));

	return sqrt(stdevx * stdevx + stdevy * stdevy);
}
