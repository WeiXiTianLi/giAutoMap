#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "ATM_Matrix.h"

class ATM_ObjectList
{
	int _maxNumber=0;
	int *_x = nullptr;
	int *_y = nullptr;
	int _orderPtr = 0;
public:
	ATM_ObjectList();
	ATM_ObjectList(int maxNumber);
	~ATM_ObjectList();
	//ATM_ObjectList(const ATM_ObjectList &objlist) = delete;
	ATM_ObjectList& operator=(const ATM_ObjectList& objList);

	void append(int x, int y);

	int x(int i);
	int y(int i);
	cv::Point p(int i);
};

class ATM_ObjectFlag
{
	int _maxNumber = 0;
	std::vector<int> _x;
	std::vector<int> _y;
	int _orderPtr = 0;
public:
	ATM_ObjectFlag();
	~ATM_ObjectFlag();

	void append(int x, int y);
	void deleteId(int id);

	int x(int i);
	int y(int i);
	cv::Point p(int i);

	int getSize();
	void clear();
};

class ATM_ObjectLists
{
	// 在此：添加新的物品需改数量
	/*******************************/
	/******/
	int _objectListsNumber = 14;
	int _objectMaxNumbers[14] = { 65,131,95,80,55,169,104,36,17,120,33,75,69 ,8531};
	bool _isShow[14] = { false };
	/******/
	/*******************************/
	bool _isShowFlag = false;
	ATM_ObjectList *_objList = nullptr;
	ATM_ObjectFlag _objFlag;

	std::vector<std::vector<std::vector<int>>> _objLists;

	int funCount = 0;
public:
	/*******************************/
	/******/
	ATM_Matrix _collectionStateFST = ATM_Matrix(1, 65);
	ATM_Matrix _collectionStateYST = ATM_Matrix(1, 131);
	ATM_Matrix _collectionStateLST = ATM_Matrix(1, 95);

	ATM_Matrix _collectionStateFHYS = ATM_Matrix(1, 80);
	
	ATM_Matrix _collectionStateLST2 = ATM_Matrix(1, 55);//150-95= 55

	ATM_Matrix _collectionStateSJK = ATM_Matrix(1, 169);
	ATM_Matrix _collectionStateSB = ATM_Matrix(1, 104);
	ATM_Matrix _collectionStateLLBH = ATM_Matrix(1, 36);
	ATM_Matrix _collectionStateQX = ATM_Matrix(1, 17);
	ATM_Matrix _collectionStateLLD = ATM_Matrix(1, 120);
	ATM_Matrix _collectionStateGGG = ATM_Matrix(1, 33);
	ATM_Matrix _collectionStateXDC = ATM_Matrix(1, 75);
	ATM_Matrix _collectionStateXL = ATM_Matrix(1, 69);
	ATM_Matrix _collectionStateBX_ALL = ATM_Matrix(1, 8531);
	/******/
	// 坐标映射比例：天理坐标系 : 程序坐标系 = 2.56 ：1
	// 坐标系原点映射： 璃月 -> 左上角 Point(1428, 2937);
	/*******************************/


	ATM_Matrix _collectionStateFlag;
	/*******************************/
	/******/
	ATM_Matrix *collectionState[14];
	/******/
	/*******************************/
	std::vector<int> visualStarKlassList;
	std::vector<int> visualStarIdList;
	std::vector<double> visualStarDisList;

	bool isSelect = false;
	int selectID = -1;

	bool isSelectObj = false;
	int selectObjKlass = -1;
	int selectObjID = -1;


	ATM_ObjectLists();
	~ATM_ObjectLists();
	void Init();
	int x(int klass, int i);
	int y(int klass, int i);
	cv::Point p(int klass, int i);
	cv::Point fp(int i);
	int objectListsNumber();
	int objectsNumber(int klass);
	int flagNumber();
	bool isShow(int klass);
	bool isShowFlag();
	void setShow(int klass);
	void setShowFlag();
	void setShow(int klass, bool isShow);
	void setShowFlag(bool isShow);

	void appendFlag(int x, int y);
	void deleteFlag(int id);
	void reAppendFlag();
	void reDeleteFlag();

	//0 未发现 1 未发现即已收集 2 已发现未收集  
	void setCollectionState(int klass, int i, int state);
	//0 未发现 1 未发现即已收集 2 已发现未收集  
	int getCollectionState(int klass, int i);
	void copyFrom(int klass, ATM_Matrix &mat);
	void copyTo(int klass, ATM_Matrix *mat);

private:
	/*******************************/
	void Init0();
	void Init1();
	void Init2();
	void Init3();
	void Init4();
	void Init5();
	void Init6();
	void Init7();
	void Init8();
	void Init9();
	void Init10();
	void Init11();
	void Init12();
	void Init13();
	/*******************************/
	//void InitId(int id);
	int getFunCount();
	void append(int i, int x, int y);
};
