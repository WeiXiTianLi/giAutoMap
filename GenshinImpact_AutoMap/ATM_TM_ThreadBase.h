#pragma once
#include <thread>
#include <Windows.h>
#include <functional>
#include <opencv2/opencv.hpp>

template <class FunOut, class FunIn, class _Obj >
class ATM_TM_ThreadBase
{
	//ʵ���̵߳ĵ�������

	//�߳�ʵ��
	std::thread *tLoopWork = nullptr;

	//��������
	//FunOut(_Obj::*ptr)(FunIn funIn) = nullptr;
	std::function<FunOut(FunIn)> fptr = nullptr;

	//�����������
	FunOut *_funOut = nullptr;
	FunIn *_funIn = nullptr;

	//�Ƿ��˳��߳�
	bool isExitThread = false;
	//�Ƿ���ڹ�������
	bool isExistFunction = false;
	//�Ƿ����й���
	bool isRunWork = false;
	//�Ƿ��������
	bool isEndWork = false;

	//�߳�ִ��ʵ��
	void run()
	{
		//ѭ������ֱ���Ƿ��˳��߳�Ϊ�棬Ӧ��ֻ����Ҫ����ʱ�˳�
		while (isExitThread == false)
		{
			//�Ƿ����й���Ϊ�沢�ҹ���������Ϊ��
			if (isRunWork && fptr != nullptr)
			{
				//�Ƿ��������Ϊ�٣�������������
				isEndWork = false;

				//Ӧ���ȼ���

				//ִ�к���
				*_funOut = fptr(*_funIn);

				//Ӧ���ͷ���

				//�Ƿ����й���Ϊ�٣�ÿ������ֻ����һ��ѭ��
				isRunWork = false;
				//�Ƿ��������Ϊ�棬�����ѽ���
				isEndWork = true;
			}
			else
			{
				//����1ms
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
	};

public:
	//bool isInit = false;
	//�޲ι���
	ATM_TM_ThreadBase()
	{
		//�����߳�ʵ�壬���������߳�ִ��ʵ��
		tLoopWork = new std::thread(&ATM_TM_ThreadBase::run, this);
	};

	//���н������һ�ι�������
	~ATM_TM_ThreadBase()
	{
		//����߳�ʵ�岻Ϊ��
		if (tLoopWork != nullptr)
		{
			//Ӧ����ȡ��
			//�����Ƿ��˳��߳�Ϊ��
			isExitThread = true;

			//�����߳�ʵ��
			tLoopWork->join();
			//�����߳�ʵ��
			delete tLoopWork;
		}
	}

	//��������������
	ATM_TM_ThreadBase(std::function<FunOut(FunIn)> funPtr)
	{
		//ֱ�����ù�������
		fptr = funPtr;
		//�����߳�ʵ�壬���������߳�ִ��ʵ��
		tLoopWork = new std::thread(&ATM_TM_ThreadBase::run, this);
	}

	//���ù�������
	void setFunction(std::function<FunOut(FunIn)> funPtr)
	{
		//���ڹ�����
		if (isRunWork)
		{
			fptr = funPtr;
		}
	}

	//virtual void workfun() {}

	//�޲�������ֱ������
	void start()
	{
		isRunWork = true;
		isEndWork = false;
	}

	//�����������ʼ����
	void start(FunIn funIn)
	{
		_funIn= funIn;
		isRunWork = true;
		isEndWork = false;
	}

	//�����������
	void setFunIn(FunIn funIn)
	{
		_funIn = funIn;
	}

	//��ȡ���
	FunOut getFunOut()
	{
		return _funOut;
	}

	//�����Ƿ����
	bool isEnd()
	{
		return isEndWork;
	}

	bool getWorkOut(FunOut &out)
	{
		if (isEnd())
		{
			out = getFunOut();
			return true;
		}
		return false;
	}
};
//typedef ATM_TM_ThreadBase<bool, Mat> ATM_TM_toBool;
//typedef ATM_TM_ThreadBase<int, Mat> ATM_TM_toInt;
//typedef ATM_TM_ThreadBase<double, Mat> ATM_TM_toDouble;
//typedef ATM_TM_ThreadBase<Point2d, Mat> ATM_TM_toPoint;
//typedef ATM_TM_ThreadBase<vector<Point2d>, Mat> ATM_TM_toVecPoint;
