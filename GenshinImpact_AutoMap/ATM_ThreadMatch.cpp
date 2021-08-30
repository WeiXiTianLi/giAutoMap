#include "ATM_ThreadMatch.h"

ATM_ThreadMatch::~ATM_ThreadMatch()
{
	if (tSurfMapInit != nullptr)
	{
		tSurfMapInit->join();
		delete tSurfMapInit;
	}
	if (tSurfMapMatch != nullptr)
	{
		tSurfMapMatch->join();
		delete tSurfMapMatch;
	}
	if (tTemplatePaimonMatch != nullptr)
	{
		tTemplatePaimonMatch->join();
		delete tTemplatePaimonMatch;
	}
	if (tOrbAvatarInit != nullptr)
	{
		tOrbAvatarInit->join();
		delete tOrbAvatarInit;
	}
	if (tOrbAvatarMatch != nullptr)
	{
		tOrbAvatarMatch->join();
		delete tOrbAvatarMatch;
	}
	if (tTemplateUIDInit != nullptr)
	{
		tTemplateUIDInit->join();
		delete tTemplateUIDInit;
	}
	if (tTemplateUIDMatch != nullptr)
	{
		tTemplateUIDMatch->join();
		delete tTemplateUIDMatch;
	}
}

void ATM_ThreadMatch::cThreadInit()
{
}

void ATM_ThreadMatch::cThreadSurfMapInit(Mat &Map)
{
	if (tSurfMapInit == nullptr && surfMap.isInit == false)
	{
		cvtColor(Map, mapGray, CV_RGB2GRAY);

		tSurfMapInit = new thread(&ATM_ThreadMatch::thread_SurfMapInit, this, ref(mapGray));
		tIsEndSurfMapInit = false;
	}
}

void ATM_ThreadMatch::cThreadSurfMapMatch()
{
	if (tSurfMapMatch == nullptr && tIsEndSurfMapInit && isExistObjMinMap && isPaimonVisial)
	{
		tSurfMapMatch = new thread(&ATM_ThreadMatch::thread_SurfMapMatch, this, ref(objMinMap));
		tIsEndSurfMapMatch = false;
	}
}

void ATM_ThreadMatch::setSurfMap(Mat mapMat)
{
	surfMap.setMap(mapMat);
}

void ATM_ThreadMatch::cThreadTemplatePaimonMatch(Mat & Template)
{
	if (tTemplatePaimonMatch == nullptr && isExistObjPaimon)
	{
		cvtColor(Template, templatePaimon, CV_RGB2GRAY);
		tTemplatePaimonMatch = new thread(&ATM_ThreadMatch::thread_TemplatePaimonMatch, this, ref(templatePaimon), ref(objPaimon));
		tIsEndTemplatePaimonMatch = false;
	}
}

void ATM_ThreadMatch::setTemplatePaimon(Mat TemplatePaimonMat)
{
	TemplatePaimonMat.copyTo(templatePaimon);
}

void ATM_ThreadMatch::setPaimon(Mat PaimonMat)
{
	PaimonMat.copyTo(objPaimon);
}

void ATM_ThreadMatch::cThreadOrbAvatarInit(Mat & TemplateAvatar)
{
	if (tOrbAvatarInit == nullptr && orbAvatar.isInit == false)
	{
		//templateAvatar = Mat(300, 300, CV_8UC1,Scalar(0,0,0));
		//cvtColor(TemplateAvatar(Rect(TemplateAvatar.cols/2-16, TemplateAvatar.rows/2-16,32,32)), templateAvatar(Rect(134,134,32,32)), CV_RGB2GRAY);
		//threshold(templateAvatar, templateAvatar, 185, 255, THRESH_TOZERO);
		////resize(templateAvatar, templateAvatar, Size(0, 0), 1, 1, 3);//INTER_CUBIC INTER_AREAz
		resize(TemplateAvatar, templateAvatar, Size(150, 150), 0, 0, INTER_LANCZOS4);//INTER_CUBIC INTER_AREAz
		tOrbAvatarInit = new thread(&ATM_ThreadMatch::thread_OrbAvatarInit, this, ref(templateAvatar));
		tIsEndOrbAvatarInit = false;
	}
}

void ATM_ThreadMatch::cThreadOrbAvatarMatch()
{
	if (tOrbAvatarMatch == nullptr && tIsEndOrbAvatarInit && isExistObjMinMap && isPaimonVisial)
	{
		//Mat temp = Mat(300, 300, CV_8UC1, Scalar(0, 0, 0));
		//objAvatar.copyTo(temp(Rect(138, 138, 24, 24)));
		//objAvatar = temp;
		//threshold(objAvatar, objAvatar, 185, 255, THRESH_TOZERO);
		////resize(objAvatar, objAvatar, Size(0, 0), 1*1.3, 1*1.3, 3);//INTER_CUBIC INTER_AREAz

		//resize(objAvatar, objAvatar, Size(150, 150), 0, 0, INTER_LANCZOS4);//INTER_CUBIC INTER_AREAz
		tOrbAvatarMatch = new thread(&ATM_ThreadMatch::thread_OrbAvatarMatch, this, ref(objAvatar));
		tIsEndOrbAvatarMatch = false;
	}
}

void ATM_ThreadMatch::setAvatat(Mat AvatarMat)
{
	AvatarMat.copyTo(objAvatar);
}

void ATM_ThreadMatch::cThreadTemplateUIDInit(Mat * TemplateUID)
{
	if (tTemplateUIDInit == nullptr && tempUID.isInit == false)
	{
		templateUID = TemplateUID;
		tTemplateUIDInit = new thread(&ATM_ThreadMatch::thread_TemplateUIDInit, this, ref(templateUID));
		tIsEndTemplateUIDInit = false;
	}
}

void ATM_ThreadMatch::cThreadTemplateUIDMatch()
{
	if (tTemplateUIDMatch == nullptr && isExistObjUID)
	{
		tTemplateUIDMatch = new thread(&ATM_ThreadMatch::thread_TemplateUIDMatch, this, ref(objUID));
		tIsEndTemplateUIDMatch = false;
	}
}

void ATM_ThreadMatch::setUID(Mat UIDMat)
{
	UIDMat.copyTo(objUID);
}

void ATM_ThreadMatch::cThreadTemplateStarInit(Mat & TemplateStar)
{
	if (tTemplateStarInit == nullptr && tempStar.isInit == false)
	{
		//templateStar = TemplateStar;
		cvtColor(TemplateStar, templateStar, CV_RGB2GRAY);
		tTemplateStarInit = new thread(&ATM_ThreadMatch::thread_TemplateStarInit, this, ref(templateStar));
		tIsEndTemplateStarInit = false;
	}
}

void ATM_ThreadMatch::cThreadTemplateStarMatch()
{
	if (tTemplateStarMatch == nullptr && isExistObjMinMap && isStarExist)
	{
		tTemplateStarMatch = new thread(&ATM_ThreadMatch::thread_TemplateStarMatch, this, ref(objStar));
		tIsEndTemplateStarMatch = false;
	}
}

void ATM_ThreadMatch::setTemplateStar(Mat TemplateStarMat)
{
	TemplateStarMat.copyTo(templateStar);
}

void ATM_ThreadMatch::setStar(Mat StarMat)
{
	StarMat.copyTo(objStar);
}

void ATM_ThreadMatch::getObjMinMap(Mat & obj)
{
	//obj.copyTo(objMinMap);
	cvtColor(obj, objMinMap, CV_RGB2GRAY);
	int Avatar_Rect_x = cvRound(obj.cols*0.4);
	int Avatar_Rect_y = cvRound(obj.rows*0.4);
	int Avatar_Rect_w = cvRound(obj.cols*0.2);
	int Avatar_Rect_h = cvRound(obj.rows*0.2);

	obj(cv::Rect(Avatar_Rect_x, Avatar_Rect_y, Avatar_Rect_w, Avatar_Rect_h)).copyTo(objAvatar);

	//obj(Rect(obj.cols / 2 - 14, obj.rows / 2 - 14, 28, 28)).copyTo(objAvatar);
	//obj(Rect(36, 36, obj.cols - 72, obj.rows - 72)).copyTo(objStar);
	cvtColor(obj(Rect(36, 36, obj.cols - 72, obj.rows - 72)), objStar, CV_RGBA2GRAY);
	isExistObjMinMap = true;
}

void ATM_ThreadMatch::getObjPaimon(Mat & obj)
{
	obj.copyTo(objPaimon);
	isExistObjPaimon = true;
}

void ATM_ThreadMatch::getObjUID(Mat & obj)
{
	obj.copyTo(objUID);
	isExistObjUID = true;
}

void ATM_ThreadMatch::CheckThread()
{
	if (tIsEndSurfMapInit == false)
	{
		CheckThread_SurfMapInit();
	}
	if (tIsEndSurfMapMatch == false)
	{
		CheckThread_SurfMapMatch();
	}
	if (tIsEndTemplatePaimonMatch == false)
	{
		CheckThread_TemplatePaimonMatch();
	}
	if (tIsEndOrbAvatarInit == false)
	{
		CheckThread_OrbAvatarInit();
	}
	if (tIsEndOrbAvatarMatch == false)
	{
		CheckThread_OrbAvatarMatch();
	}
	if (tIsEndTemplateUIDInit == false)
	{
		CheckThread_TemplateUIDInit();
	}
	if (tIsEndTemplateUIDMatch == false)
	{
		CheckThread_TemplateUIDMatch();
	}
	if (tIsEndTemplateStarInit == false)
	{
		CheckThread_TemplateStarInit();
	}
	if (tIsEndTemplateStarMatch == false)
	{
		CheckThread_TemplateStarMatch();
	}
}

void ATM_ThreadMatch::CheckThread_SurfMapInit()
{
	DWORD exitCode;
	if (tSurfMapInit != nullptr)
	{
		GetExitCodeThread(tSurfMapInit->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tSurfMapInit->join();
			delete tSurfMapInit;
			tSurfMapInit = nullptr;
			tIsEndSurfMapInit = true;
		}
	}
}

void ATM_ThreadMatch::thread_SurfMapInit(Mat & tar)
{
	surfMap.setMap(tar);
	surfMap.Init();
}

void ATM_ThreadMatch::CheckThread_SurfMapMatch()
{
	DWORD exitCode;
	if (tSurfMapMatch != nullptr)
	{
		GetExitCodeThread(tSurfMapMatch->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tSurfMapMatch->join();
			delete tSurfMapMatch;
			tSurfMapMatch = nullptr;
			tIsEndSurfMapMatch = true;
		}
	}
}

void ATM_ThreadMatch::thread_SurfMapMatch(Mat & Obj)
{
	//surfMap.setMinMap(Obj);
	if (isExistObjMinMap)
	{
		//surfMap.setMinMap(objMinMap);
		surfMap.setMinMap(Obj);
		surfMap.SURFMatch();
	}
}

void ATM_ThreadMatch::CheckThread_TemplatePaimonMatch()
{
	DWORD exitCode;
	if (tTemplatePaimonMatch != nullptr)
	{
		GetExitCodeThread(tTemplatePaimonMatch->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tTemplatePaimonMatch->join();
			delete tTemplatePaimonMatch;
			tTemplatePaimonMatch = nullptr;
			tIsEndTemplatePaimonMatch = true;
		}
	}
}

void ATM_ThreadMatch::thread_TemplatePaimonMatch(Mat &Template, Mat & Obj)
{
	if (isExistObjPaimon)
	{
		tempPaimon.setPaimonTemplate(Template);
		tempPaimon.setPaimonMat(Obj);
		tempPaimon.TemplatePaimon();
	}
}

void ATM_ThreadMatch::CheckThread_OrbAvatarInit()
{
	DWORD exitCode;
	if (tOrbAvatarInit != nullptr)
	{
		GetExitCodeThread(tOrbAvatarInit->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tOrbAvatarInit->join();
			delete tOrbAvatarInit;
			tOrbAvatarInit = nullptr;
			tIsEndOrbAvatarInit = true;
		}
	}
}

void ATM_ThreadMatch::thread_OrbAvatarInit(Mat & tar)
{
	if (tar.empty());
	orbAvatar.setAvatarTemplate(templateAvatar);
	orbAvatar.Init();
}

void ATM_ThreadMatch::CheckThread_OrbAvatarMatch()
{
	DWORD exitCode;
	if (tOrbAvatarMatch != nullptr)
	{
		GetExitCodeThread(tOrbAvatarMatch->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tOrbAvatarMatch->join();
			delete tOrbAvatarMatch;
			tOrbAvatarMatch = nullptr;
			tIsEndOrbAvatarMatch = true;
		}
	}
}

void ATM_ThreadMatch::thread_OrbAvatarMatch(Mat & Obj)
{
	if (isExistObjMinMap)
	{
		orbAvatar.setAvatarMat(Obj);
		orbAvatar.ORBMatch();
	}
}

void ATM_ThreadMatch::CheckThread_TemplateUIDInit()
{
	DWORD exitCode;
	if (tTemplateUIDInit != nullptr)
	{
		GetExitCodeThread(tTemplateUIDInit->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tTemplateUIDInit->join();
			delete tTemplateUIDInit;
			tTemplateUIDInit = nullptr;
			tIsEndTemplateUIDInit = true;
		}
	}
}

void ATM_ThreadMatch::thread_TemplateUIDInit(Mat * tar)
{
	tempUID.setUIDTemplate(tar);
	tempUID.Init();
}

void ATM_ThreadMatch::CheckThread_TemplateUIDMatch()
{
	DWORD exitCode;
	if (tTemplateUIDMatch != nullptr)
	{
		GetExitCodeThread(tTemplateUIDMatch->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tTemplateUIDMatch->join();
			delete tTemplateUIDMatch;
			tTemplateUIDMatch = nullptr;
			tIsEndTemplateUIDMatch = true;
		}
	}
}

void ATM_ThreadMatch::thread_TemplateUIDMatch(Mat & Obj)
{
	if (isExistObjUID)
	{
		//surfMap.setMinMap(objMinMap);
		tempUID.setUIDMat(Obj);
		tempUID.TemplateUID();
	}
}

void ATM_ThreadMatch::CheckThread_TemplateStarInit()
{
	DWORD exitCode;
	if (tTemplateStarInit != nullptr)
	{
		GetExitCodeThread(tTemplateStarInit->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tTemplateStarInit->join();
			delete tTemplateStarInit;
			tTemplateStarInit = nullptr;
			tIsEndTemplateStarInit = true;
		}
	}
}

void ATM_ThreadMatch::thread_TemplateStarInit(Mat & tar)
{
	tempStar.setStarTemplate(tar);
	tempStar.Init();
}

void ATM_ThreadMatch::CheckThread_TemplateStarMatch()
{
	DWORD exitCode;
	if (tTemplateStarMatch != nullptr)
	{
		GetExitCodeThread(tTemplateStarMatch->native_handle(), &exitCode);
		if (exitCode == 0)
		{
			tTemplateStarMatch->join();
			delete tTemplateStarMatch;
			tTemplateStarMatch = nullptr;
			tIsEndTemplateStarMatch = true;
		}
	}
}

void ATM_ThreadMatch::thread_TemplateStarMatch(Mat & Obj)
{
	if (isExistObjMinMap)
	{
		tempStar.setStarMat(Obj);
		tempStar.TemplateStar();
	}
}

void ATM_ThreadMatch::GetMatchResults()
{
	if (tIsEndSurfMapMatch)
	{
		if (surfMap.getLocalPos().x > 0 && surfMap.getLocalPos().y > 0)
		{
			pos = surfMap.getLocalPos();
		}
	}
	if (tIsEndSurfMapMatch)
	{
		isContinuity = surfMap.getIsContinuity();
	}
	if (tIsEndTemplatePaimonMatch)
	{
		isPaimonVisial = tempPaimon.getPaimonVisible();
	}
	if (tIsEndOrbAvatarMatch)
	{
		rotationAngle = orbAvatar.getRotationAngle();
	}
	if (tIsEndTemplateUIDMatch)
	{
		uid = tempUID.getUID();
	}
	if (tIsEndTemplateStarMatch)
	{
		isStarVisible = tempStar.getStar();
		//if (isStarVisible)
		//{
		starPos = tempStar.getStarPos();
		//}
	}
}



void ATM_TM_Continuity::setState(bool state)
{
	continuity[0] = continuity[1];
	continuity[1] = continuity[2];
	continuity[2] = state;
}

bool ATM_TM_Continuity::getIsContinuity()
{
	return continuity[0] && continuity[1] && continuity[2];
}
