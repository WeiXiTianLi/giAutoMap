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
#include "ATM_TM_SurfMap.h"
#include "ATM_TM_TemplatePaimon.h"
#include "ATM_TM_ORBAvatar.h"
#include "ATM_TM_TemplateUID.h"
#include "ATM_TM_TemplateStar.h"

using namespace std;
using namespace cv;

class ATM_TM_Continuity
{
	bool continuity[3] = { false };
public:
	void setState(bool state);
	bool getIsContinuity();
};

class ATM_ThreadMatch
{
	Mat mapGray;
	Mat objMinMap;

	Mat templateAvatar;
	Mat objAvatar;

	Mat templatePaimon;
	Mat objPaimon;

	Mat templateStar;
	Mat objStar;

	Mat *templateUID;
	Mat objUID;

	ATM_TM_SurfMap surfMap;
	ATM_TM_TemplatePaimon tempPaimon;
	ATM_TM_ORBAvatar orbAvatar;
	ATM_TM_TemplateUID tempUID;
	ATM_TM_TemplateStar tempStar;
	//ATM_TM_Thread tempUID;
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
	void cThreadSurfMapInit(Mat &Map);
	void cThreadSurfMapMatch();
	void setSurfMap(Mat mapMat);

	//Template Match Paimon
	void cThreadTemplatePaimonMatch(Mat &Template);
	void setTemplatePaimon(Mat TemplatePaimonMat);
	void setPaimon(Mat PaimonMat);

	//ORB Match Avatar
	void cThreadOrbAvatarInit(Mat &TemplatAvatar);
	void cThreadOrbAvatarMatch();
	void setAvatat(Mat AvatarMat);

	//Template Match UID
	void cThreadTemplateUIDInit(Mat * TemplateUID);
	void cThreadTemplateUIDMatch();
	void setUID(Mat UIDMat);

	//Template Match Star
	void cThreadTemplateStarInit(Mat &TemplateStar);
	void cThreadTemplateStarMatch();
	void setTemplateStar(Mat TemplateStarMat);
	void setStar(Mat StarMat);

	//Set Some Mat
	void getObjMinMap(Mat &obj);
	void getObjPaimon(Mat &obj);
	void getObjUID(Mat &obj);

	//Check Thread End
	void CheckThread();

	//SURF Init
	void CheckThread_SurfMapInit();
	void thread_SurfMapInit(Mat& tar);

	//SURF Match
	void CheckThread_SurfMapMatch();
	void thread_SurfMapMatch(Mat& Obj);

	//Template Match
	void CheckThread_TemplatePaimonMatch();
	void thread_TemplatePaimonMatch(Mat &Template, Mat& Obj);

	//ORB Init
	void CheckThread_OrbAvatarInit();
	void thread_OrbAvatarInit(Mat& tar);

	//ORB Match
	void CheckThread_OrbAvatarMatch();
	void thread_OrbAvatarMatch(Mat& Obj);

	//Template UID Init
	void CheckThread_TemplateUIDInit();
	void thread_TemplateUIDInit(Mat * tar);

	//Template UID Match
	void CheckThread_TemplateUIDMatch();
	void thread_TemplateUIDMatch(Mat& Obj);

	//Template Star Init
	void CheckThread_TemplateStarInit();
	void thread_TemplateStarInit(Mat & tar);

	//Template Star Match
	void CheckThread_TemplateStarMatch();
	void thread_TemplateStarMatch(Mat& Obj);

	void GetMatchResults();
};

