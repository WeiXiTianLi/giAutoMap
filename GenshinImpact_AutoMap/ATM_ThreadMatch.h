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
#include "ATM_TM_ThreadBase.h"
#include "ATM_TM_SurfMap.h"
#include "ATM_TM_TemplatePaimon.h"
#include "ATM_TM_ORBAvatar.h"
#include "ATM_TM_TemplateUID.h"
#include "ATM_TM_TemplateStar.h"

class ATM_TM_Continuity
{
	bool continuity[3] = { false };
public:
	void setState(bool state);
	bool getIsContinuity();
};

class ATM_ThreadMatch
{
	cv::Mat mapGray;
	cv::Mat objMinMap;

	cv::Mat templateAvatar;
	cv::Mat objAvatar;

	cv::Mat templatePaimon;
	cv::Mat objPaimon;

	cv::Mat templateStar;
	cv::Mat objStar;

	cv::Mat *templateUID;
	cv::Mat objUID;

	ATM_TM_SurfMap surfMap;
	ATM_TM_TemplatePaimon tempPaimon;
	ATM_TM_ORBAvatar orbAvatar;
	ATM_TM_TemplateUID tempUID;
	ATM_TM_TemplateStar tempStar;
	//ATM_TM_Thread tempUID;

	//ATM_TM_toBool tWork_Paimon;
	//ATM_TM_TemplatePaimonT paimonT;
	//ATM_TM_ThreadBase<bool, Mat> tWork_Paimon2(tempPaimon.getPaimonVisible(Mat));
	//ATM_TM_toPoint test;
public:
	~ATM_ThreadMatch();
	ATM_TM_Continuity continuityState;
	//Point pos = Point(1466, 3272);
	Point2d pos = Point2d(5352, 9432);
	bool isPaimonVisial = false;
	bool isContinuity = false;
	double rotationAngle = 0;
	int uid = 0;
	bool isStarExist = false;
	bool isStarVisible = false;
	vector<Point2d> starPos;


	bool isExistObjMinMap = false;
	bool isExistObjPaimon = false;
	bool isExistObjUID = false;

	bool tIsEndSurfMapInit = false;
	bool tIsEndSurfMapMatch = false;
	bool tIsEndTemplatePaimonMatch = false;
	bool tIsEndOrbAvatarInit = false;
	bool tIsEndOrbAvatarMatch = false;
	bool tIsEndTemplateUIDInit = false;
	bool tIsEndTemplateUIDMatch = false;
	bool tIsEndTemplateStarInit = false;
	bool tIsEndTemplateStarMatch = false;

	thread * tSurfMapInit = nullptr;
	thread * tSurfMapMatch = nullptr;
	thread * tTemplatePaimonMatch = nullptr;
	thread * tOrbAvatarInit = nullptr;
	thread * tOrbAvatarMatch = nullptr;	
	thread * tTemplateUIDInit = nullptr;
	thread * tTemplateUIDMatch = nullptr;
	thread * tTemplateStarInit = nullptr;
	thread * tTemplateStarMatch = nullptr;

	//Init
	void cThreadInit();

	//SURF Match Map
	void cThreadSurfMapInit(cv::Mat &Map);
	void cThreadSurfMapMatch();
	void setSurfMap(cv::Mat mapMat);

	//Template Match Paimon
	void cThreadTemplatePaimonMatch(cv::Mat &Template);
	void setTemplatePaimon(cv::Mat TemplatePaimonMat);
	void setPaimon(cv::Mat PaimonMat);

	//ORB Match Avatar
	void cThreadOrbAvatarInit(cv::Mat &TemplatAvatar);
	void cThreadOrbAvatarMatch();
	void setAvatat(cv::Mat AvatarMat);

	//Template Match UID
	void cThreadTemplateUIDInit(cv::Mat * TemplateUID);
	void cThreadTemplateUIDMatch();
	void setUID(cv::Mat UIDMat);

	//Template Match Star
	void cThreadTemplateStarInit(cv::Mat &TemplateStar);
	void cThreadTemplateStarMatch();
	void setTemplateStar(cv::Mat TemplateStarMat);
	void setStar(cv::Mat StarMat);

	//Set Some Mat
	void getObjMinMap(cv::Mat &obj);
	void getObjPaimon(cv::Mat &obj);
	void getObjUID(cv::Mat &obj);

	//Check Thread End
	void CheckThread();

	//SURF Init
	void CheckThread_SurfMapInit();
	void thread_SurfMapInit(cv::Mat& tar);

	//SURF Match
	void CheckThread_SurfMapMatch();
	void thread_SurfMapMatch(cv::Mat& Obj);

	//Template Match
	void CheckThread_TemplatePaimonMatch();
	void thread_TemplatePaimonMatch(cv::Mat &Template, cv::Mat& Obj);

	//ORB Init
	void CheckThread_OrbAvatarInit();
	void thread_OrbAvatarInit(cv::Mat& tar);

	//ORB Match
	void CheckThread_OrbAvatarMatch();
	void thread_OrbAvatarMatch(cv::Mat& Obj);

	//Template UID Init
	void CheckThread_TemplateUIDInit();
	void thread_TemplateUIDInit(cv::Mat * tar);

	//Template UID Match
	void CheckThread_TemplateUIDMatch();
	void thread_TemplateUIDMatch(cv::Mat& Obj);

	//Template Star Init
	void CheckThread_TemplateStarInit();
	void thread_TemplateStarInit(cv::Mat & tar);

	//Template Star Match
	void CheckThread_TemplateStarMatch();
	void thread_TemplateStarMatch(cv::Mat& Obj);

	void GetMatchResults();
};

