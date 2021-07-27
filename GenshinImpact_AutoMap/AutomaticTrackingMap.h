#pragma once
#include <QtWidgets/QMainWindow>
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <vector>
#include "ATM_Resource.h"
#include "ATM_MouseEvent.h"
#include "ATM_GiState.h"
#include "ATM_ThreadMatch.h"
#include "ATM_ObjectLists.h"
#include "ATM_SendSocket.h"
#include "ATM_ActivationKYJG.h"
#include "ATM_SaveLoadFile.h"
#include "ATM_Modules.h"

enum ThisWinState
{
	Normal,
	Minimize,
	NoTop,
	Top,
	TopShow,
	Wait,
};

class AutomaticTrackingMap
{
	//������Դ
	ATM_Resource RES;
	//ԭ�񴰿���Ϣ
	ATM_GiState GIS;
	//���߳�ƥ��
	ATM_ThreadMatch TMS;
	//������Ʒ���
	ATM_ObjectLists OLS;
	//Socketͨ��
	ATM_SendSocket SST;
	//��ӫ�ƹݼ���
	ATM_ActivationKYJG AKY;
	//�浵
	ATM_SaveLoadFile SLF;

	cv::Mat MainMat;
	cv::Mat MainMatTmp;

	int thisStateMode = 0;
	int thisStateModeNext = 0;
	bool isFristChangeThisState = true;
	bool isUpdataDraw = true;
public:
	AutomaticTrackingMap();
	~AutomaticTrackingMap();
public:

	//���λ��
	ATM_MouseEvent MET;

	//�Զ����������
	HWND thisHandle=NULL;
	//�Ƿ������Զ�׷��
	bool isAutoMode = false;
	bool isAutoInitFinish = false;

	//���ڴ�С
	Size autoWindowSize = Size(300, 300);
	//��������С
	Size autoMapSize = Size(300, 300);//Size(212, 212);
	Point autoMapCenter = Point(150, 150);//Point(106,106);
	//������ͼ��С
	Size mapSize = Size(RES.GIMAP.cols, RES.GIMAP.rows);
	//������ͼ�������ģ����ԭ��
	Point mapWorldCenter = Point(1416, 3306);
	//Point mapWorldCenter = Point(5352, 9432);

	//�������������Դ��ͼλ��
	Point2d zerosMinMap = Point(1466, 3272);//Point(5352, 9432)
	//Point2d zerosMinMap = Point(5352, 9432);//Point(5352, 9432)
	//���������ԭ�񴰿�λ��
	Point offGiMinMap = Point(250, 100);
	//
	Rect minMapRect;

	//ԭ���Ƿ��������б�־

	string SystemUserName="";
	string SystemUserLocalLow="";
	string SystemUserFileIndex="";
	string ApplicationCompanyName = "����ϵͳ";
	string ApplicationName="�������Զ���ͼ";
	LANGID SystemLanguageID=0;
	string SystemLanguage = "Chinese";
	//Qt
	QImage MainImg;

	void Init(HWND mapWindowsHandle);
	void Exit();

	void FrontEndUpdata();
	void BackEndUpdata();

	void Mat2QImage();

	void CustomProcess(int i);

	void updataDraw();
public:
	Mat getViewMap();

	void getGiState();
	void setThisState_Normal();
	void setThisState_Minimize();
	void setThisState_Top();
	void setThisState_TopShow();

	void getThisOffset();

	void getGiHandle();
	void getThisHandle();
	void getThisHandle(HWND _thisHandle);

	void setWindowsPos();
	void setWindowsPos(HWND _thisHandle);

	void setMoveMapDownPos(int x,int y);
	void setMoveMapUpPos(int x, int y);
	void setMoveMapMovePos(int x, int y);

	void setOffsetDownPos(int x, int y);
	void setOffsetUpPos(int x, int y);
	void setOffsetMovePos(int x, int y);

	void setScaleMapDelta(int x, int y,int delta);

	void setAutoMode();
	bool getAutoMode();

	void setObjIsShow(int klass);
	void setObjFlagIsShow();
	void setAddFlagOnPos();
	void setKongYingJiuGuanState();

	void setGenshinImpactWndHandle(HWND giHandle);

	void testSaveScreen();

	int getUID();
	void saveLocal();
	void loadLocal();

private:
	void getSystemInfo();
private:
	int getThisState();
	void getKYJGState();
	void setThisState();
	void setThreadMatchMat();

	void drawStarObjectLists();
	void drawObjectLists();
	void drawFlagLists();
	void drawAvatar();
	void CopyToThis();
	void CopyToLocal();
private:
	void addWeightedAlpha(Mat &backgroundImage, Mat& Image, Mat &maskImage);
	void addWeightedAlpha(Mat &backgroundImage, Mat& Image, Mat &maskImage,double alpha);
	void addWeightedPNG(Mat &backgroundImage, Mat& Image);
	Mat rotateAvatar(double angle);
	Mat rotateAvatar(double angle,double scale);
};

typedef AutomaticTrackingMap ATmap;

