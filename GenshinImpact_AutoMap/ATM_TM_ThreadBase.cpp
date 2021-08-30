#include "ATM_TM_ThreadBase.h"

void ATM_TM_ThreadBase::run()
{
	while (isExitThread == false)
	{
		if (isRunWork && (*ptr) != nullptr)
		{
			//ptr(workInput);
			isRunWork = false;
			isEndWork = true;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

ATM_TM_ThreadBase::ATM_TM_ThreadBase()
{
	tLoopWork = new thread(&ATM_TM_ThreadBase::run, this);
}

ATM_TM_ThreadBase::~ATM_TM_ThreadBase()
{
	if (tLoopWork != nullptr)
	{
		isExitThread = true;
		tLoopWork->join();
		delete tLoopWork;
	}
}

//ATM_TM_ThreadBase::ATM_TM_ThreadBase(void(*funPtr)(Mat &inMat))
//{
//	setFunction(funPtr);
//	tLoopWork = new thread(&ATM_TM_ThreadBase::run, this);
//}
//
//void ATM_TM_ThreadBase::setFunction(void(*funPtr)(Mat &inMat))
//{
//	ptr = funPtr;
//	isExistFunction = true;
//}

void ATM_TM_ThreadBase::workfun()
{
}

void ATM_TM_ThreadBase::start(Mat & inMat)
{
	if (isExistFunction == false)
	{
		throw"Not Found Work Function";
	}
	//workInput = inMat;
	isRunWork = true;
	isEndWork = false;
}

bool ATM_TM_ThreadBase::isEnd()
{
	return isEndWork;
}
