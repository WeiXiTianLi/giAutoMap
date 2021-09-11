#pragma once
#include <thread>
#include <Windows.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

template <class FunOut,class FunIn>
class ATM_TM_ThreadBase
{
	//实现线程的单独控制

	//线程实体
	thread *tLoopWork = nullptr;

	//工作函数
	FunOut*(*ptr)(FunIn *funIn) = nullptr;

	//输入输出变量
	FunOut *_funOut = nullptr;
	FunIn *_funIn = nullptr;

	//是否退出线程
	bool isExitThread = false;
	//是否存在工作函数
	bool isExistFunction = false;
	//是否运行工作
	bool isRunWork = false;
	//是否结束工作
	bool isEndWork = false;

	//线程执行实体
	void run()
	{
		//循环事务直到是否退出线程为真，应当只有需要析构时退出
		while (isExitThread == false)
		{
			//是否运行工作为真并且工作函数不为空
			if (isRunWork && (*ptr) != nullptr)
			{
				//是否结束工作为假，工作已运行中
				isEndWork = false;

				//应当先加锁

				//执行函数
				_funOut = ptr(_funIn);

				//应当释放锁

				//是否运行工作为假，每次运行只运行一个循环
				isRunWork = false;
				//是否结束工作为真，工作已结束
				isEndWork = true;
			}
			else
			{
				//休眠1ms
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
	};

public:
	//bool isInit = false;
	ATM_TM_ThreadBase()
	{
		//构造线程实体，并且运行线程执行实体
		tLoopWork = new thread(&ATM_TM_ThreadBase::run, this);
	};
	~ATM_TM_ThreadBase()
	{
		//如果线程实体不为空
		if (tLoopWork != nullptr)
		{
			//应当先取锁
			//设置是否退出线程为真
			isExitThread = true;

			//结束线程实体
			tLoopWork->join();
			//析构线程实体
			delete tLoopWork;
		}
	}

	ATM_TM_ThreadBase(FunOut(*funPtr)(FunIn &funIn))
	{
		//直接设置工作函数
		fptr = funPtr;
		//构造线程实体，并且运行线程执行实体
		tLoopWork = new thread(&ATM_TM_ThreadBase::run, this);
	}

	//void setFunction(FunOut(*funPtr)(FunIn &funIn))
	//{
	//	//设置工作函数
	//	if (isRunWork && (*ptr) != nullptr)
	//	{

	//	}
	//	fptr = funPtr;
	//}

	//virtual void workfun() {}
	void start()
	{
		isRunWork = true;
		isEndWork = false;
	}

	void start(FunIn & funIn)
	{
		//if (isExistFunction == false)
		//{
		//	throw"Not Found Work Function";
		//}
		////workInput = funIn;
		isRunWork = true;
		isEndWork = false;
	}

	void setFunIn(FunIn funIn)
	{
		_funIn = funIn;
	}

	FunOut getFunOut()
	{
		return _funOut;
	}

	bool isEnd()
	{
		return isEndWork;
	}
};
typedef ATM_TM_ThreadBase<bool, Mat> ATM_TM_toBool;
typedef ATM_TM_ThreadBase<int, Mat> ATM_TM_toInt;
typedef ATM_TM_ThreadBase<double, Mat> ATM_TM_toDouble;
typedef ATM_TM_ThreadBase<Point2d, Mat> ATM_TM_toPoint;
typedef ATM_TM_ThreadBase<vector<Point2d>, Mat> ATM_TM_toVecPoint;
