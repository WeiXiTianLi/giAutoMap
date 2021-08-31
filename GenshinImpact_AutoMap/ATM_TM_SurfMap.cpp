#include "ATM_TM_SurfMap.h"

ATM_TM_SurfMap::ATM_TM_SurfMap()
{
	hisP[0] = Point();
	hisP[1] = Point();
	hisP[2] = Point();

	KF = KalmanFilter(stateNum, measureNum, 0);
	state = Mat(stateNum, 1, CV_32F); //state(x,y,detaX,detaY)
	processNoise= Mat(stateNum, 1, CV_32F);
	measurement = Mat::zeros(measureNum, 1, CV_32F);	//measurement(x,y)

	randn(state, Scalar::all(0), Scalar::all(0.1)); //随机生成一个矩阵，期望是0，标准差为0.1;
	KF.transitionMatrix = (Mat_<float>(4, 4) <<
		1, 0, 1, 0,
		0, 1, 0, 1,
		0, 0, 1, 0,
		0, 0, 0, 1);//元素导入矩阵，按行;

	//setIdentity: 缩放的单位对角矩阵;
	//!< measurement matrix (H) 观测模型
	setIdentity(KF.measurementMatrix);

	//!< process noise covariance matrix (Q)
	// wk 是过程噪声，并假定其符合均值为零，协方差矩阵为Qk(Q)的多元正态分布;
	setIdentity(KF.processNoiseCov, Scalar::all(1e-5));

	//!< measurement noise covariance matrix (R)
	//vk 是观测噪声，其均值为零，协方差矩阵为Rk,且服从正态分布;
	setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));

	//!< priori error estimate covariance matrix (P'(k)): P'(k)=A*P(k-1)*At + Q)*/  A代表F: transitionMatrix
	//预测估计协方差矩阵;
	setIdentity(KF.errorCovPost, Scalar::all(1));

	//!< corrected state (x(k)): x(k)=x'(k)+K(k)*(z(k)-H*x'(k))
//initialize post state of kalman filter at random 
	randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));

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
				//				if (_minMapLastMat.empty() == false)
				//				{
				//					Mat someMap(_minMapLastMat);//上一帧小地图画面
				//					Mat minMap(img_object);//当前帧小地图画面
				//
				//					detectorSomeMap = xfeatures2d::SURF::create(minHessian);
				//					detectorSomeMap->detectAndCompute(someMap, noArray(), Kp_SomeMap, Dp_SomeMap);
				//					detectorSomeMap->detectAndCompute(minMap, noArray(), Kp_MinMap, Dp_MinMap);
				//
				//					//没检测到特征点 连续性断开
				//					if (Kp_SomeMap.size() == 0 || Kp_MinMap.size() == 0)
				//					{
				//						isContinuity = false;
				//					}
				//					else
				//					{
				//						Ptr<DescriptorMatcher> matcherTmp = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
				//						std::vector< std::vector<DMatch> > KNN_mTmp;
				//#ifdef _DEBUG
				//						std::vector<DMatch> good_matchesTmp;
				//#endif
				//						matcherTmp->knnMatch(Dp_MinMap, Dp_SomeMap, KNN_mTmp, 2);
				//						std::vector<double> lisx;
				//						std::vector<double> lisy;
				//						double sumx = 0;
				//						double sumy = 0;
				//						for (size_t i = 0; i < KNN_mTmp.size(); i++)
				//						{
				//							if (KNN_mTmp[i][0].distance < ratio_thresh * KNN_mTmp[i][1].distance)
				//							{
				//#ifdef _DEBUG
				//								good_matchesTmp.push_back(KNN_mTmp[i][0]);
				//#endif
				//								// 这里有个bug回卡进来，进入副本或者切换放大招时偶尔触发
				//								if (KNN_mTmp[i][0].queryIdx >= Kp_MinMap.size())
				//								{
				//									continue;
				//								}
				//
				//								//mapScale 考虑城镇和大世界两种
				//								lisx.push_back(((minMap.cols / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.x)*1.0 + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.x));
				//								lisy.push_back(((minMap.rows / 2.0 - Kp_MinMap[KNN_mTmp[i][0].queryIdx].pt.y)*1.0 + Kp_SomeMap[KNN_mTmp[i][0].trainIdx].pt.y));
				//								sumx += lisx.back();
				//								sumy += lisy.back();
				//							}
				//						}
				//
				//#ifdef _DEBUG
				//						Mat img_matches, imgmap, imgminmap;
				//						drawKeypoints(someMap, Kp_SomeMap, imgmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
				//						drawKeypoints(img_object, Kp_MinMap, imgminmap, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
				//
				//						drawMatches(img_object, Kp_MinMap, someMap, Kp_SomeMap, good_matchesTmp, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
				//#endif
				//
				//						if (max(lisx.size(), lisy.size()) > 4)
				//						{
				//							if (min(lisx.size(), lisy.size()) >= 10)
				//							{
				//								isOnCity = true;
				//							}
				//							else
				//							{
				//								isOnCity = false;
				//							}
				//							//if(max(lisx.size(), lisy.size()) <=10 )
				//							//{
				//							//	isOnCity = false;
				//							//}
				//
				//							Point2d p = Point2d(sumx / (double)lisx.size(), sumy / lisy.size()); //均值
				//							//double meany = sumy / lisy.size(); //均值
				//							//Point2d p = SPC(lisx, sumx, lisy, sumy);
				//							
				//							double x = (p.x - someMap.cols / 2.0);
				//							double y = (p.y - someMap.rows / 2.0);
				//
				//							pos = Point2d(x + hisP[2].x, y + hisP[2].y);
				//							minMap.copyTo(_minMapLastMat);
				//						}
				//					}
				//
				//
				//				}
				//				else
				//				{
				//					Mat minMap(img_object);//当前帧小地图画面
				//					minMap.copyTo(_minMapLastMat);
				//				}


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
						Ptr<DescriptorMatcher> matcherTmp = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
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
								Ptr<DescriptorMatcher> matcherTmp = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
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
						Ptr<DescriptorMatcher> matcherTmp = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
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
			Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
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
	if (isConveying)
	{
		KF.init(stateNum, measureNum, 0);

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
	}
	else
	{
		//Point statePt = Point((int)KF.statePost.at<float>(0), (int)KF.statePost.at<float>(1));

//2.kalman prediction   
		Mat prediction = KF.predict();
		Point2d predictPt = Point2d(prediction.at<float>(0), prediction.at<float>(1));

		//3.update measurement
		measurement.at<float>(0, 0) = static_cast<float>(pos.x);
		measurement.at<float>(1, 0) = static_cast<float>(pos.y);

		//4.update
		KF.correct(measurement);

		pos = Point2d(KF.statePost.at<float>(0), KF.statePost.at<float>(1));
	}




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
