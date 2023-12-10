#include "ATM_SaveLoadFile.h"
#include<Windows.h>
using namespace std;

ATM_SaveLoadFile::ATM_SaveLoadFile()
{
}

ATM_SaveLoadFile::~ATM_SaveLoadFile()
{
}

void ATM_SaveLoadFile::setIndex(string index)
{
	if (_access(index.c_str(), 0) == -1)
	{
		int e = _mkdir(index.c_str());//error 3
		if (e == -1 && GetLastError() == 3)
		{
			int i = 0;
			string _index(index);
			i = index.rfind("\\", (int)index.size() - 2);
			_index.erase(_index.begin() + i, _index.end());
			_mkdir(_index.c_str());
			_mkdir(index.c_str());
		}
	}
	_saveIndex = index;
	getFilePath();
}

void ATM_SaveLoadFile::setUID(int uid)
{
	if (_uid == uid)
	{
		return;
	}
	_uid = uid;
	getUIDStr();
	getFilePath();
	if (tryLoad())
	{
		load();
	}
	else
	{
		save();
	}
}

void ATM_SaveLoadFile::save()
{
	FILE *fpSave;
	errno_t err;
	err = fopen_s(&fpSave, _uidFilePath.c_str(), "w");
	if (fpSave == NULL)
	{
		return;
	}

	fprintf_s(fpSave, "%s\n", _uidStr.c_str());
	fprintf_s(fpSave, "%s\n", getSystemTime().c_str());

	saveMatrix(fpSave, _stateFST);
	saveMatrix(fpSave, _stateYST);
	saveMatrix(fpSave, _stateLST);
	saveMatrix(fpSave, _stateFHYS);
	//saveMatrix(fpSave, _stateLST2);
	saveMatrix(fpSave, _stateFlag);

	fclose(fpSave);
}

void ATM_SaveLoadFile::load()
{
	FILE *fpLoad;
	errno_t err;
	err = fopen_s(&fpLoad, _uidFilePath.c_str(), "r");
	if (fpLoad == NULL)
	{
		return;
	}

	fscanf_s(fpLoad, "%d\n", &_uid);
	//fscanf_s(fpLoad, "%[^\n]s", &_lastSaveTimeChars, 60);
	fgets(_lastSaveTimeChars, 60, fpLoad);

	loadMatrix(fpLoad, _stateFST);
	loadMatrix(fpLoad, _stateYST);

	/*
	loadMatrix(fpLoad, _stateLST);
	loadMatrix(fpLoad, _stateFHYS);

	*/
	int x = 0, y = 0;
	fscanf_s(fpLoad, "%d %d", &x, &y);
	if (_stateLST.row() != y)
	{
		ATM_Matrix & mat = _stateFHYS;
		for (int i = 0; i < mat.col(); i++)
		{
			for (int ii = 0; ii < mat.row(); ii++)
			{
				fscanf_s(fpLoad, "%d", &mat[i][ii]);
			}
		}
	}
	else
	{
		ATM_Matrix & mat = _stateLST;
		for (int i = 0; i < mat.col(); i++)
		{
			for (int ii = 0; ii < mat.row(); ii++)
			{
				fscanf_s(fpLoad, "%d", &mat[i][ii]);
			}
		}
		loadMatrix(fpLoad, _stateFHYS);
	}

	loadMatrix(fpLoad, _stateFlag);

	fclose(fpLoad);
}

//ATM_Matrix * ATM_SaveLoadFile::StateClass(int klass)
//{
//	//switch (klass)
//	//{
//	//	case 0:
//	//	{
//	//		return _stateFST;
//	//	}
//	//	default:
//	//		break;
//	//}
//	return nullptr;
//}

bool ATM_SaveLoadFile::tryLoad()
{
	if (_access(_uidFilePath.c_str(), 0) == -1)
	{
		return false;
	}
	else
	{
		return true;
	}
	return false;
}

void ATM_SaveLoadFile::logError(string mes)
{
	if (_access(logFilePath.c_str(), 0) == -1)
	{
		string _index(logFilePath);
		int i = logFilePath.rfind("\\", (int)logFilePath.size() - 2);
		_index.erase(_index.begin() + i, _index.end());
		_mkdir(_index.c_str());
	}

	FILE *fpSave;
	errno_t err;
	err = fopen_s(&fpSave, logFilePath.c_str(), "a");
	if (fpSave == NULL)
	{
		return;
	}

	fprintf_s(fpSave, "%s | LOG ERROR   | message: %s |\n",
		getSystemTime().c_str(), mes.c_str());

	fclose(fpSave);

}

void ATM_SaveLoadFile::logExeRun()
{
	if (_access(logFilePath.c_str(), 0) == -1)
	{
		string _index(logFilePath);
		int i = logFilePath.rfind("\\", (int)logFilePath.size() - 2);
		_index.erase(_index.begin() + i, _index.end());
		_mkdir(_index.c_str());
	}
	FILE *fpSave;
	errno_t err;
	err = fopen_s(&fpSave, logFilePath.c_str(), "a");
	if (fpSave == NULL)
	{
		return;
	}

	fprintf_s(fpSave, "%s | LOG EXE     | exe run |\n", getSystemTime().c_str());

	fclose(fpSave);

}

void ATM_SaveLoadFile::logExeExit()
{
	if (_access(logFilePath.c_str(), 0) == -1)
	{
		string _index(logFilePath);
		int i = logFilePath.rfind("\\", (int)logFilePath.size() - 2);
		_index.erase(_index.begin() + i, _index.end());
		_mkdir(_index.c_str());
	}
	FILE *fpSave;
	errno_t err;
	err = fopen_s(&fpSave, logFilePath.c_str(), "a");
	if (fpSave == NULL)
	{
		return;
	}

	fprintf_s(fpSave, "%s | LOG EXE     | exe exit |\n", getSystemTime().c_str());

	fclose(fpSave);

}

void ATM_SaveLoadFile::logUIDEvent(int newUID)
{
	if (_access(logFilePath.c_str(), 0) == -1)
	{
		string _index(logFilePath);
		int i = logFilePath.rfind("\\", (int)logFilePath.size() - 2);
		_index.erase(_index.begin() + i, _index.end());
		_mkdir(_index.c_str());
	}
	FILE *fpSave;
	errno_t err;
	err = fopen_s(&fpSave, logFilePath.c_str(), "a");
	if (fpSave == NULL)
	{
		return;
	}

	fprintf_s(fpSave, "%s | LOG UID     | UID_%d to UID_%d |\n",
		getSystemTime().c_str(), _uid, newUID);

	fclose(fpSave);

}

void ATM_SaveLoadFile::logAvatarEvent(double x, double y, double a)
{
	static double _x = 0;
	static double _y = 0;
	static double _a = 0;
	if (abs(a - _a) < 1e-5 && abs(x - _x) < 1e-5 && abs(y - _y) < 1e-5)
	{
		_x = x;
		_y = y;
		_a = a;
		return;
	}
	if (_access(logFilePath.c_str(), 0) == -1)
	{
		string _index(logFilePath);
		int i = logFilePath.rfind("\\", (int)logFilePath.size() - 2);
		_index.erase(_index.begin() + i, _index.end());
		_mkdir(_index.c_str());
	}
	FILE *fpSave;
	errno_t err;
	err = fopen_s(&fpSave, logFilePath.c_str(), "a");
	if (fpSave == NULL)
	{
		return;
	}
	
	fprintf_s(fpSave, "%s | LOG AVATAR  | x: %lf y: %lf a: %lf |\n",
		getSystemTime().c_str(), x, y, a);

	_x = x;
	_y = y;
	_a = a;

	fclose(fpSave);

}

void ATM_SaveLoadFile::logStarEvent(int starClass, int starId, int starState)
{
	if (_access(logFilePath.c_str(), 0) == -1)
	{
		string _index(logFilePath);
		int i = logFilePath.rfind("\\", (int)logFilePath.size() - 2);
		_index.erase(_index.begin() + i, _index.end());
		_mkdir(_index.c_str());
	}
	FILE *fpSave;
	errno_t err;
	err = fopen_s(&fpSave, logFilePath.c_str(), "a");
	if (fpSave == NULL)
	{
		return;
	}

	fprintf_s(fpSave, "%s | LOG STAR    | starClass: %d starId: %d starState: %d |\n",
		getSystemTime().c_str(), starClass, starId, starState);

	fclose(fpSave);

}

void ATM_SaveLoadFile::logStarEvent(int starClass, int starId, int x, int y)
{
	if (_access(logFilePath.c_str(), 0) == -1)
	{
		string _index(logFilePath);
		int i = logFilePath.rfind("\\", (int)logFilePath.size() - 2);
		_index.erase(_index.begin() + i, _index.end());
		_mkdir(_index.c_str());
	}
	FILE *fpSave;
	errno_t err;
	err = fopen_s(&fpSave, logFilePath.c_str(), "a");
	if (fpSave == NULL)
	{
		return;
	}

	fprintf_s(fpSave, "%s | LOG STAR    | starCoor updata starClass: %d starId: %d x: %d y: %d|\n",
		getSystemTime().c_str(), starClass, starId, x, y);

	fclose(fpSave);
}

void ATM_SaveLoadFile::getUIDStr()
{
	char uidBuff[10];
	sprintf_s(uidBuff, "%09d", _uid);
	_uidStr = uidBuff;
}

void ATM_SaveLoadFile::getFilePath()
{
	getUIDStr();
	_uidFilePath = _saveIndex + _uid_ + _uidStr + ".ini";
	logFilePath = _saveIndex + "诸域周游.记";
}

string ATM_SaveLoadFile::getSystemTime()
{
	struct tm t;
	time_t nowTime = time(NULL);
	char nowTimeChars[64] = { 0 };
	localtime_s(&t, &nowTime);
	strftime(nowTimeChars, sizeof(nowTimeChars) - 1, "%Y-%m-%d %H:%M:%S", &t);     //年-月-日 时-分-秒

	return string(nowTimeChars);
}

void ATM_SaveLoadFile::saveMatrix(FILE * fpSave, ATM_Matrix & mat)
{
	fprintf_s(fpSave, "%d %d\n", mat.col(), mat.row());
	for (int i = 0; i < mat.col(); i++)
	{
		for (int ii = 0; ii < mat.row(); ii++)
		{
			fprintf_s(fpSave, "%d ", mat[i][ii]);
		}
		fprintf_s(fpSave, "\n");
	}
}

void ATM_SaveLoadFile::loadMatrix(FILE * fpLoad, ATM_Matrix & mat)
{
	int x = 0, y = 0;
	fscanf_s(fpLoad, "%d %d", &x, &y);
	mat.reSet(x, y);
	for (int i = 0; i < mat.col(); i++)
	{
		for (int ii = 0; ii < mat.row(); ii++)
		{
			fscanf_s(fpLoad, "%d", &mat[i][ii]);
		}
	}
}

bool ATM_SaveLoadFile::loadMatrix2(FILE * fpLoad, ATM_Matrix & mat)
{
	int x = 0, y = 0;
	fscanf_s(fpLoad, "%d %d", &x, &y);
	if (mat.row() != y)
	{
		return false;
	}
	mat.reSet(x, y);
	for (int i = 0; i < mat.col(); i++)
	{
		for (int ii = 0; ii < mat.row(); ii++)
		{
			fscanf_s(fpLoad, "%d", &mat[i][ii]);
		}
	}
	return true;
}