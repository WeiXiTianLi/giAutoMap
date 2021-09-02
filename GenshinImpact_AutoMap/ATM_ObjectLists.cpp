#include "ATM_ObjectLists.h"

ATM_ObjectList::ATM_ObjectList()
{
}

ATM_ObjectList::ATM_ObjectList(int maxNumber) :_maxNumber(maxNumber)
{
	_x = new int[_maxNumber];
	_y = new int[_maxNumber];
}

ATM_ObjectList::~ATM_ObjectList()
{
	if (_x != nullptr)
	{
		delete[] _x;
	}
	if (_y != nullptr)
	{
		delete[] _y;
	}
}

ATM_ObjectList & ATM_ObjectList::operator=(const ATM_ObjectList & objList)
{
	// TODO: 在此处插入 return 语句
	if (this == &objList)
	{
		return *this;
	}
	//delete
	if (_x != nullptr)
	{
		delete[] _x;
	}
	if (_y != nullptr)
	{
		delete[] _y;
	}
	this->_maxNumber = objList._maxNumber;
	this->_orderPtr = objList._orderPtr;
	this->_x = new int[_maxNumber];
	this->_y = new int[_maxNumber];
	for (int i = 0; i < objList._maxNumber; i++)
	{
		this->_x[i] = objList._x[i];
		this->_y[i] = objList._y[i];
	}
	return *this;
}

void ATM_ObjectList::append(int x, int y)
{
	if (_orderPtr >= _maxNumber)
	{
		throw "List Full";
	}
	_x[_orderPtr] = x+11;//+11;
	_y[_orderPtr] = y-288;//-163;//369
	_orderPtr++;
}

int ATM_ObjectList::x(int i)
{
	if (i < 0)
	{
		return _x[0];
	}
	if (i >= _maxNumber)
	{
		return _x[_maxNumber - 1];
	}
	return _x[i];
}

int ATM_ObjectList::y(int i)
{
	if (i < 0)
	{
		return _y[0];
	}
	if (i >= _maxNumber)
	{
		return _y[_maxNumber - 1];
	}
	return _y[i];
}

cv::Point ATM_ObjectList::p(int i)
{
	if (i < 0)
	{
		return cv::Point(_x[0], _y[0]);
	}
	if (i >= _maxNumber)
	{
		return cv::Point(_x[_maxNumber - 1], _y[_maxNumber - 1]);
	}
	return cv::Point(_x[i], _y[i]);
}

ATM_ObjectFlag::ATM_ObjectFlag()
{

}

ATM_ObjectFlag::~ATM_ObjectFlag()
{
}

void ATM_ObjectFlag::append(int x, int y)
{
	_x.push_back(x);
	_y.push_back(y);
	_maxNumber++;
}

void ATM_ObjectFlag::deleteId(int id)
{
	if (id < _maxNumber)
	{
		_x.erase(_x.begin() + id);
		_y.erase(_y.begin() + id);
		_maxNumber--;
	}

}

int ATM_ObjectFlag::x(int i)
{
	if (i < 0)
	{
		return _x[0];
	}
	if (i >= _maxNumber)
	{
		return _x[_maxNumber - 1];
	}
	return _x[i];
}

int ATM_ObjectFlag::y(int i)
{
	if (i < 0)
	{
		return _y[0];
	}
	if (i >= _maxNumber)
	{
		return _y[_maxNumber - 1];
	}
	return _y[i];
}

cv::Point ATM_ObjectFlag::p(int i)
{
	if (i < 0)
	{
		return cv::Point(_x[0], _y[0]);
	}
	if (i >= _maxNumber)
	{
		return cv::Point(_x[_maxNumber - 1], _y[_maxNumber - 1]);
	}
	return cv::Point(_x[i], _y[i]);
}

int ATM_ObjectFlag::getSize()
{
	return _maxNumber;
}

void ATM_ObjectFlag::clear()
{
	_maxNumber = 0;
	_orderPtr = 0;
	_x.clear();
	_y.clear();
}


ATM_ObjectLists::ATM_ObjectLists()
{
	_objList = new ATM_ObjectList[_objectListsNumber];
	for (int i = 0; i < _objectListsNumber; i++)
	{
		_objList[i] = ATM_ObjectList(_objectMaxNumbers[i]);
	}

	collectionState[0] = &_collectionStateFST;
	collectionState[1] = &_collectionStateYST;
	collectionState[2] = &_collectionStateLST;
	collectionState[3] = &_collectionStateFHYS;
	collectionState[4] = &_collectionStateSJK;
	collectionState[5] = &_collectionStateSB;
	collectionState[6] = &_collectionStateLLBH;
	collectionState[7] = &_collectionStateQX;
	collectionState[8] = &_collectionStateLLD;
	collectionState[9] = &_collectionStateGGG;
	collectionState[10] = &_collectionStateXDC;
	collectionState[11] = &_collectionStateXL;

	Init();
}

ATM_ObjectLists::~ATM_ObjectLists()
{
	if (_objList != nullptr)
	{
		delete[] _objList;
	}
}

void ATM_ObjectLists::Init()
{
	Init0();
	Init1();
	Init2();
	Init3();
	Init4();
	Init5();
	Init6();
	Init7();
	Init8();
	Init9();
	Init10();
}

int ATM_ObjectLists::x(int klass, int i)
{
	return _objList[klass].x(i);
}

int ATM_ObjectLists::y(int klass, int i)
{
	return _objList[klass].y(i);
}

cv::Point ATM_ObjectLists::p(int klass, int i)
{
	return _objList[klass].p(i);
}

cv::Point ATM_ObjectLists::fp(int i)
{
	return _objFlag.p(i);
}

int ATM_ObjectLists::objectListsNumber()
{
	return _objectListsNumber;
}

int ATM_ObjectLists::objectsNumber(int klass)
{
	if (klass >= _objectListsNumber)
	{
		throw "Element Out Of Bounds";
	}
	return _objectMaxNumbers[klass];
}

int ATM_ObjectLists::flagNumber()
{
	return _objFlag.getSize();
}

bool ATM_ObjectLists::isShow(int klass)
{
	if (klass >= _objectListsNumber)
	{
		throw "Element Out Of Bounds";
	}
	return _isShow[klass];
}

bool ATM_ObjectLists::isShowFlag()
{
	return _isShowFlag;
}

void ATM_ObjectLists::setShow(int klass)
{
	if (klass >= _objectListsNumber)
	{
		throw "Element Out Of Bounds";
	}
	_isShow[klass] = !_isShow[klass];
}

void ATM_ObjectLists::setShowFlag()
{
	_isShowFlag = !_isShowFlag;
}

void ATM_ObjectLists::setShow(int klass, bool isShow)
{
	if (klass >= _objectListsNumber)
	{
		throw "Element Out Of Bounds";
	}
	_isShow[klass] = isShow;
}

void ATM_ObjectLists::setShowFlag(bool isShow)
{
	_isShowFlag = isShow;
}

void ATM_ObjectLists::appendFlag(int x, int y)
{
	_objFlag.append(x, y);
	if (_collectionStateFlag.row() <= _objFlag.getSize())
	{
		ATM_Matrix stateFlagTmp = ATM_Matrix(2, _objFlag.getSize());
		for (int i = 0; i < _collectionStateFlag.col(); i++)
		{
			for (int ii = 0; ii < _collectionStateFlag.row(); ii++)
			{
				stateFlagTmp[i][ii] = _collectionStateFlag[i][ii];
			}
		}
		_collectionStateFlag = stateFlagTmp;
	}
	_collectionStateFlag.set(0, _objFlag.getSize() - 1, x);
	_collectionStateFlag.set(1, _objFlag.getSize() - 1, y);
}

void ATM_ObjectLists::deleteFlag(int id)
{
	_objFlag.deleteId(id);
	if (_collectionStateFlag.row() > _objFlag.getSize())
	{
		_collectionStateFlag = ATM_Matrix(2, _objFlag.getSize());
		for (int ii = 0; ii < _collectionStateFlag.row(); ii++)
		{
			_collectionStateFlag[0][ii] = _objFlag.x(ii);
			_collectionStateFlag[1][ii] = _objFlag.y(ii);
		}
	}
}

void ATM_ObjectLists::setCollectionState(int klass, int i, int state)
{
	(*collectionState[klass]).set(0, i, state);
}

int ATM_ObjectLists::getCollectionState(int klass, int i)
{
	return (*collectionState[klass]).at(0, i);
}

void ATM_ObjectLists::copyFrom(int klass, ATM_Matrix & mat)
{
	mat.copyTo((*collectionState[klass]));
	/*************/
	if (klass == 4)
	{
		reAppendFlag();
	}
}

void ATM_ObjectLists::copyTo(int klass, ATM_Matrix * mat)
{
	(collectionState[klass])->copyTo(*mat);
}

void ATM_ObjectLists::Init0()
{
	int i = 0;
	{
		append(i, 1628, 747);
		append(i, 1754, 535);
		append(i, 1494, 570);
		append(i, 1660, 583);
		append(i, 1878, 761);
		append(i, 1749, 620);
		append(i, 1591, 726);
		append(i, 1549, 709);
		append(i, 1592, 679);
		append(i, 1425, 821);
		append(i, 1468, 794);
		append(i, 1522, 887);
		append(i, 1778, 1014);
		append(i, 1890, 925);
		append(i, 1711, 1115);
		append(i, 1988, 957);
		append(i, 2006, 849);
		append(i, 2064, 886);
		append(i, 2069, 827);
		append(i, 2030, 766);
		append(i, 2023, 720);
		append(i, 2114, 760);
		append(i, 2063, 991);
		append(i, 2112, 1050);
		append(i, 2058, 1108);
		append(i, 2090, 1190);
		append(i, 2158, 1271);
		append(i, 2312, 1192);
		append(i, 2273, 1377);
		append(i, 2046, 1435);
		append(i, 1948, 1515);
		append(i, 2076, 1555);
		append(i, 2389, 1316);
		append(i, 2474, 1314);
		append(i, 2536, 1444);
		append(i, 2601, 1468);
		append(i, 2606, 1511);
		append(i, 2649, 1601);
		append(i, 2684, 1395);
		append(i, 2802, 1471);
		append(i, 2841, 1551);
		append(i, 2813, 1631);
		append(i, 2979, 1671);
		append(i, 2867, 1874);
		append(i, 3050, 1781);
		append(i, 2958, 1749);
		append(i, 3229, 1661);
		append(i, 2593, 1318);
		append(i, 2712, 1342);
		append(i, 2780, 1368);
		append(i, 2722, 1292);
		append(i, 2822, 1251);
		append(i, 2719, 1207);
		append(i, 2718, 1015);
		append(i, 2950, 1069);
		append(i, 3115, 996);
		append(i, 3010, 866);
		append(i, 2665, 659);
		append(i, 2591, 550);
		append(i, 2967, 622);
		append(i, 3141, 509);
		append(i, 3592, 770);
		append(i, 3790, 1543);
		append(i, 2259, 749);
		append(i, 2296, 797);
	}
}

void ATM_ObjectLists::Init1()
{
	int i = 1;
	{
		append(i, 1139, 1159);
		append(i, 1081, 1345);
		append(i, 1067, 1295);
		append(i, 1243, 1303);
		append(i, 1158, 1378);
		append(i, 1310, 1421);
		append(i, 1366, 1394);
		append(i, 1194, 1429);
		append(i, 1046, 1461);
		append(i, 1129, 1493);
		append(i, 1277, 1483);
		append(i, 1307, 1501);
		append(i, 1388, 1507);
		append(i, 1254, 1580);
		append(i, 1036, 1563);
		append(i, 1114, 1641);
		append(i, 1142, 1622);
		append(i, 1240, 1684);
		append(i, 1222, 1760);
		append(i, 1270, 1641);
		append(i, 1440, 1612);
		append(i, 1372, 1667);
		append(i, 1364, 1720);
		append(i, 1381, 1805);
		append(i, 1437, 1700);
		append(i, 1576, 1592);
		append(i, 1495, 1757);
		append(i, 1571, 1826);
		append(i, 1390, 1997);
		append(i, 1278, 1933);
		append(i, 1833, 1872);
		append(i, 1466, 2122);
		append(i, 1441, 2117);
		append(i, 1513, 2167);
		append(i, 1605, 2080);
		append(i, 1913, 2082);
		append(i, 1838, 2132);
		append(i, 1917, 2164);
		append(i, 1795, 2176);
		append(i, 1788, 2239);
		append(i, 1952, 2246);
		append(i, 2060, 2466);
		append(i, 1901, 2375);
		append(i, 2082, 2297);
		append(i, 2121, 2339);
		append(i, 2082, 2448);
		append(i, 1881, 2534);
		append(i, 2324, 2512);
		append(i, 2173, 2918);
		append(i, 2430, 2905);
		append(i, 2519, 2989);
		append(i, 2304, 3090);
		append(i, 1711, 2312);
		append(i, 1679, 2395);
		append(i, 1355, 2471);
		append(i, 1561, 2564);
		append(i, 1365, 2617);
		append(i, 1564, 2611);
		append(i, 1621, 2636);
		append(i, 1810, 2617);
		append(i, 1459, 2726);
		append(i, 1555, 2805);
		append(i, 1835, 2791);
		append(i, 1754, 2869);
		append(i, 1359, 2998);
		append(i, 1464, 2950);
		append(i, 1534, 2923);
		append(i, 1164, 2740);
		append(i, 1161, 2556);
		append(i, 918, 2948);
		append(i, 989, 2959);
		append(i, 1030, 2945);
		append(i, 758, 3012);
		append(i, 836, 3086);
		append(i, 736, 3218);
		append(i, 881, 3248);
		append(i, 725, 3322);
		append(i, 1042, 3214);
		append(i, 1267, 3144);
		append(i, 1234, 3163);
		append(i, 1237, 3232);
		append(i, 1183, 3283);
		append(i, 1304, 3349);
		append(i, 1332, 3414);
		append(i, 1115, 3483);
		append(i, 996, 3480);
		append(i, 735, 3504);
		append(i, 609, 2702);
		append(i, 603, 2595);
		append(i, 820, 1569);
		append(i, 709, 1699);
		append(i, 756, 1734);
		append(i, 924, 1717);
		append(i, 715, 1866);
		append(i, 1026, 1915);
		append(i, 1317, 2212);
		append(i, 1215, 2178);
		append(i, 1087, 2505);
		append(i, 1054, 2336);
		append(i, 1868, 3340);
		append(i, 988, 2212);
		append(i, 888, 2320);
		append(i, 811, 2378);
		append(i, 747, 2312);
		append(i, 778, 2246);
		append(i, 720, 2255);
		append(i, 704, 2225);
		append(i, 643, 2332);
		append(i, 612, 2169);
		append(i, 753, 2082);
		append(i, 790, 2022);
		append(i, 697, 2049);
		append(i, 486, 1852);
		append(i, 444, 1962);
		append(i, 467, 2063);
		append(i, 367, 2074);
		append(i, 439, 2089);
		append(i, 471, 2107);
		append(i, 523, 2128);
		append(i, 355, 2242);
		append(i, 286, 2214);
		append(i, 280, 2308);
		append(i, 318, 2295);
		append(i, 410, 2262);
		append(i, 429, 2262);
		append(i, 1451, 1857);
		append(i, 1938, 2358);
		append(i, 1199, 2245);
		append(i, 979, 1771);
		append(i, 2620, 3136);
		append(i, 1295, 2113);
	}
}

void ATM_ObjectLists::Init2()
{
	int i = 2;
	{
		append(i, 4774, 4776);
		append(i, 5223, 4973);
		append(i, 5317, 5460);
		append(i, 4995, 5111);
		append(i, 4974, 4985);
		append(i, 4241, 5157);
		append(i, 4070, 5682);
		append(i, 4105, 5852);
		append(i, 3442, 5788);
		append(i, 3291, 5644);
		append(i, 3431, 5649);
		append(i, 3266, 5560);
		append(i, 3792, 5544);
		append(i, 3927, 5660);
		append(i, 3900, 5586);
		append(i, 3590, 5800);
		append(i, 3508, 5863);
		append(i, 5355, 4817);
		append(i, 5167, 4760);
		append(i, 5062, 4634);
		append(i, 5070, 4523);
		append(i, 5592, 4544);
		append(i, 5230, 4886);
		append(i, 5144, 4872);
		append(i, 5213, 4831);
		append(i, 5134, 4820);
		append(i, 5066, 4804);
		append(i, 4938, 4746);
		append(i, 4809, 5361);
		append(i, 3563, 6053);
		append(i, 3601, 6010);
		append(i, 3627, 5964);
		append(i, 3647, 6010);
		append(i, 3312, 5837);
		append(i, 3469, 6057);
		append(i, 3508, 5970);
		append(i, 3728, 5756);
		append(i, 3636, 5665);
		append(i, 4441, 4498);
		append(i, 3428, 5576);
		append(i, 4171, 5839);
		append(i, 4183, 5723);
		append(i, 4271, 5659);
		append(i, 4438, 5432);
		append(i, 4460, 5325);
		append(i, 4292, 5383);
		append(i, 4923, 5028);
		append(i, 4914, 5027);
		append(i, 2981, 5738);
		append(i, 3135, 5809);
		append(i, 3369, 5971);
		append(i, 3550, 5734);
		append(i, 4050, 5745);
		append(i, 4080, 5795);
		append(i, 4133, 5681);
		append(i, 4201, 5640);
		append(i, 4230, 5645);
		append(i, 4247, 5790);
		append(i, 4256, 5843);
		append(i, 4123, 5777);
		append(i, 4895, 5178);
		append(i, 5030, 5457);
		append(i, 5055, 4649);
		append(i, 4994, 4760);
		append(i, 5087, 4720);
		append(i, 5141, 4675);
		append(i, 5366, 4442);
		append(i, 5260, 4455);
		append(i, 5138, 4947);
		append(i, 5172, 4802);
		append(i, 4202, 5718);
		append(i, 4109, 5523);
		append(i, 4068, 5491);
		append(i, 3687, 6222);
		append(i, 4577, 5625);
		append(i, 4132, 5782);
		append(i, 4184, 5464);
		append(i, 5154, 4746);
		append(i, 5232, 4729);
		append(i, 5205, 5099);
		append(i, 5343, 5433);
		append(i, 4629, 5455);
		append(i, 3430, 5928);
		append(i, 4081, 5851);
		append(i, 3635, 5781);
		append(i, 3640, 5915);
		append(i, 3650, 5848);
		append(i, 3361, 5761);
		append(i, 4172, 5708);
		append(i, 5260, 4781);
		append(i, 5349, 4751);
		append(i, 4855, 4877);
		append(i, 5482, 4652);
		append(i, 3619, 5906);
		append(i, 4845, 4940);

	}
}

void ATM_ObjectLists::Init3()
{
	int i = 3;
	{
		append(i, 2442, 1953);
		append(i, 2625, 2089);
		append(i, 2438, 2183);
		append(i, 2400, 2085);
		append(i, 2447, 2056);
		append(i, 2405, 2024);
		append(i, 2363, 2005);
		append(i, 2625, 2113);
		append(i, 2114, 2005);
		append(i, 2152, 1906);
		append(i, 2105, 1841);
		append(i, 2419, 1911);
		append(i, 2386, 1930);
		append(i, 2353, 1953);
		append(i, 2452, 2052);
		append(i, 2438, 2033);
		append(i, 2428, 2042);
		append(i, 2405, 2047);
		append(i, 2339, 2005);
		append(i, 2372, 2010);
		append(i, 2021, 1855);
		append(i, 2133, 1958);
		append(i, 2293, 1930);
		append(i, 2278, 1967);
		append(i, 2264, 1930);
		append(i, 2161, 1874);
		append(i, 2161, 1902);
		append(i, 2016, 1977);
		append(i, 2002, 1977);
		append(i, 2063, 1977);
		append(i, 2035, 2047);
		append(i, 2180, 2070);
		append(i, 2171, 2085);
		append(i, 2147, 2103);
		append(i, 2161, 2117);
		append(i, 2203, 2253);
		append(i, 2236, 2183);
		append(i, 2307, 2094);
		append(i, 2307, 2122);
		append(i, 2424, 2014);
		append(i, 2419, 1986);
		append(i, 2410, 1991);
		append(i, 2433, 2094);
		append(i, 2466, 2085);
		append(i, 2461, 2089);
		append(i, 2339, 2141);
		append(i, 2564, 2000);
		append(i, 2635, 2010);
		append(i, 2597, 2038);
		append(i, 2522, 2038);
		append(i, 2428, 2117);
		append(i, 2536, 2094);
		append(i, 2438, 2178);
		append(i, 2410, 2300);
		append(i, 2635, 2342);
		append(i, 2588, 2258);
		append(i, 2588, 2234);
		append(i, 2789, 2164);
		append(i, 2649, 2117);
		append(i, 2742, 2113);
		append(i, 2860, 2033);
		append(i, 2681, 1981);
		append(i, 2653, 1958);
		append(i, 2714, 1972);
		append(i, 2480, 1897);
		append(i, 2485, 1958);
		append(i, 2499, 1921);
		append(i, 2546, 1878);
		append(i, 2686, 1813);
		append(i, 2616, 1888);
		append(i, 2588, 1846);
		append(i, 2541, 1813);
		append(i, 2358, 1789);
		append(i, 2480, 1794);
		append(i, 2457, 1757);
		append(i, 2372, 1710);
		append(i, 2288, 1742);
		append(i, 2138, 1719);
		append(i, 2105, 1733);
		append(i, 2222, 1630);
	}
}

void ATM_ObjectLists::Init4()
{
	int i = 4;
	{
	append(i, 2276, 2882);
	append(i, 2273, 2886);
	append(i, 2277, 2887);
	append(i, 2386, 3033);
	append(i, 2383, 3038);
	append(i, 2337, 3082);
	append(i, 2335, 3087);
	append(i, 2347, 3108);
	append(i, 2368, 3164);
	append(i, 2365, 3169);
	append(i, 2363, 3180);
	append(i, 2370, 3172);
	append(i, 2369, 3177);
	append(i, 2368, 3179);
	append(i, 2365, 3195);
	append(i, 1120, 1626);
	append(i, 1122, 1630);
	append(i, 1164, 1600);
	append(i, 1167, 1605);
	append(i, 1169, 1608);
	append(i, 1172, 1612);
	append(i, 925, 3521);
	append(i, 926, 3525);
	append(i, 735, 3511);
	append(i, 877, 3334);
	append(i, 876, 3340);
	append(i, 879, 3338);
	append(i, 1023, 3327);
	append(i, 1043, 3349);
	append(i, 1049, 3350);
	append(i, 1115, 3428);
	append(i, 1115, 3441);
	append(i, 1124, 3443);
	append(i, 1274, 3253);
	append(i, 1271, 3256);
	append(i, 1274, 3257);
	append(i, 1286, 3289);
	append(i, 1288, 3288);
	append(i, 1282, 3296);
	append(i, 1185, 3035);
	append(i, 1176, 3040);
	append(i, 1190, 3052);
	append(i, 1178, 3069);
	append(i, 917, 3066);
	append(i, 920, 3063);
	append(i, 909, 3075);
	append(i, 921, 3071);
	append(i, 915, 3076);
	append(i, 919, 3075);
	append(i, 849, 3096);
	append(i, 853, 3096);
	append(i, 858, 3093);
	append(i, 860, 3092);
	append(i, 651, 2768);
	append(i, 924, 2717);
	append(i, 924, 2724);
	append(i, 1224, 2600);
	append(i, 1221, 2593);
	append(i, 1215, 2597);
	append(i, 1149, 2516);
	append(i, 1150, 2519);
	append(i, 1153, 2529);
	append(i, 814, 2367);
	append(i, 451, 2546);
	append(i, 802, 2314);
	append(i, 723, 2344);
	append(i, 730, 2347);
	append(i, 656, 2310);
	append(i, 598, 2300);
	append(i, 572, 2284);
	append(i, 566, 2219);
	append(i, 516, 2231);
	append(i, 600, 2186);
	append(i, 678, 2186);
	append(i, 675, 2189);
	append(i, 686, 2180);
	append(i, 683, 2181);
	append(i, 730, 2230);
	append(i, 737, 2168);
	append(i, 760, 2130);
	append(i, 898, 2106);
	append(i, 944, 2075);
	append(i, 920, 2038);
	append(i, 810, 2033);
	append(i, 1025, 1945);
	append(i, 730, 1998);
	append(i, 669, 2025);
	append(i, 551, 2159);
	append(i, 561, 2141);
	append(i, 519, 2161);
	append(i, 584, 2054);
	append(i, 556, 2064);
	append(i, 645, 2087);
	append(i, 662, 2102);
	append(i, 504, 2181);
	append(i, 598, 2152);
	append(i, 590, 2162);
	append(i, 555, 2111);
	append(i, 624, 2065);
	append(i, 837, 2161);
	append(i, 1183, 2054);
	append(i, 461, 1953);
	append(i, 465, 1954);
	append(i, 463, 1959);
	append(i, 465, 1967);
	append(i, 454, 1968);
	append(i, 443, 1958);
	append(i, 436, 1964);
	append(i, 437, 1968);
	append(i, 910, 1886);
	append(i, 914, 1887);
	append(i, 836, 1798);
	append(i, 842, 1803);
	append(i, 650, 1752);
	append(i, 649, 1760);
	append(i, 516, 1777);
	append(i, 1004, 2606);
	append(i, 1049, 2565);
	append(i, 1050, 2569);
	append(i, 1024, 2483);
	append(i, 1026, 2529);
	append(i, 695, 2585);
	append(i, 698, 2585);
	append(i, 1313, 619);
	append(i, 1628, 473);
	append(i, 1302, 680);
	append(i, 1595, 670);
	append(i, 1617, 669);
	append(i, 1647, 672);
	append(i, 1701, 689);
	append(i, 1549, 722);
	append(i, 1535, 735);
	append(i, 1415, 723);
	append(i, 1430, 774);
	append(i, 1441, 807);
	append(i, 1451, 821);
	append(i, 1397, 834);
	append(i, 1458, 857);
	append(i, 1452, 908);
	append(i, 1538, 791);
	append(i, 1562, 796);
	append(i, 1533, 923);
	append(i, 1580, 916);
	append(i, 1560, 888);
	append(i, 1605, 939);
	append(i, 1648, 827);
	append(i, 1704, 777);
	append(i, 1656, 955);
	append(i, 2107, 734);
	append(i, 1997, 912);
	append(i, 1989, 958);
	append(i, 1761, 1203);
	append(i, 2069, 1023);
	append(i, 2057, 1118);
	append(i, 2057, 1121);
	append(i, 1999, 1202);
	append(i, 2185, 1145);
	append(i, 2222, 1235);
	append(i, 2277, 1517);
	append(i, 2286, 1512);
	append(i, 2291, 1516);
	append(i, 2293, 1518);
	append(i, 2677, 717);
	append(i, 2680, 1524);
	append(i, 2781, 712);
	append(i, 2790, 711);
	append(i, 2938, 625);
	append(i, 3003, 547);
	append(i, 3142, 1067);
}
}

void ATM_ObjectLists::Init5()
{
	int i = 5;
	{
	append(i, 787, 3528);
	append(i, 808, 3473);
	append(i, 897, 3570);
	append(i, 1071, 3516);
	append(i, 1143, 3377);
	append(i, 1038, 3369);
	append(i, 1043, 3375);
	append(i, 1039, 3322);
	append(i, 1080, 3211);
	append(i, 772, 3375);
	append(i, 957, 3058);
	append(i, 858, 3097);
	append(i, 759, 3019);
	append(i, 1077, 2942);
	append(i, 863, 2647);
	append(i, 862, 2653);
	append(i, 749, 2716);
	append(i, 721, 2671);
	append(i, 828, 2587);
	append(i, 813, 2583);
	append(i, 1001, 2595);
	append(i, 1012, 2631);
	append(i, 994, 2649);
	append(i, 1109, 2737);
	append(i, 1115, 2739);
	append(i, 1110, 2743);
	append(i, 1157, 2709);
	append(i, 1158, 2737);
	append(i, 726, 2562);
	append(i, 677, 2516);
	append(i, 792, 2502);
	append(i, 886, 2506);
	append(i, 881, 2456);
	append(i, 968, 2509);
	append(i, 964, 2514);
	append(i, 859, 2406);
	append(i, 866, 2406);
	append(i, 961, 2328);
	append(i, 982, 2310);
	append(i, 1052, 2313);
	append(i, 1023, 2203);
	append(i, 1039, 2201);
	append(i, 1104, 2205);
	append(i, 1120, 2130);
	append(i, 934, 1905);
	append(i, 984, 1896);
	append(i, 978, 1899);
	append(i, 686, 1923);
	append(i, 545, 2043);
	append(i, 542, 2048);
	append(i, 433, 1988);
	append(i, 436, 1993);
	append(i, 438, 1997);
	append(i, 308, 2206);
	append(i, 309, 2213);
	append(i, 321, 2243);
	append(i, 326, 2244);
	append(i, 298, 2265);
	append(i, 310, 2312);
	append(i, 292, 2307);
	append(i, 300, 2303);
	append(i, 293, 2314);
	append(i, 300, 2312);
	append(i, 307, 2338);
	append(i, 250, 2336);
	append(i, 249, 2263);
	append(i, 265, 2238);
	append(i, 570, 2331);
	append(i, 574, 2332);
	append(i, 651, 1873);
	append(i, 591, 1869);
	append(i, 891, 1820);
	append(i, 895, 1818);
	append(i, 779, 1618);
	append(i, 1084, 1573);
	append(i, 1085, 1580);
	append(i, 1119, 1421);
	append(i, 1125, 1425);
	append(i, 1326, 1401);
	append(i, 1330, 1398);
	append(i, 1330, 1402);
	append(i, 1422, 1454);
	append(i, 1428, 1445);
	append(i, 1346, 1470);
	append(i, 1352, 1470);
	append(i, 1370, 1668);
	append(i, 1367, 1673);
	append(i, 1900, 1741);
	append(i, 1900, 1749);
	append(i, 1912, 1731);
	append(i, 1954, 1721);
	append(i, 1953, 1731);
	append(i, 1959, 1731);
	append(i, 2008, 2253);
	append(i, 2246, 2849);
	append(i, 2456, 2961);
	append(i, 2328, 3096);
	append(i, 2484, 3020);
	append(i, 2466, 3024);
	append(i, 2467, 3043);
	append(i, 1832, 2147);
	append(i, 1309, 1905);
	append(i, 1106, 2646);
	append(i, 1100, 2648);
}
}

void ATM_ObjectLists::Init6()
{
	int i = 6;
	{
	append(i, 1507, 3337);
	append(i, 1509, 3340);
	append(i, 1393, 3309);
	append(i, 1416, 3276);
	append(i, 1368, 3300);
	append(i, 1376, 3298);
	append(i, 1398, 3293);
	append(i, 1345, 3174);
	append(i, 1359, 3181);
	append(i, 1333, 3211);
	append(i, 1363, 3218);
	append(i, 1356, 3226);
	append(i, 1352, 3230);
	append(i, 1332, 3241);
	append(i, 1386, 3239);
	append(i, 1386, 3239);
	append(i, 1386, 3247);
	append(i, 1281, 1329);
	append(i, 1263, 1328);
	append(i, 1247, 1319);
	append(i, 1244, 1327);
	append(i, 1243, 1344);
	append(i, 1210, 1322);
	append(i, 1181, 1336);
	append(i, 1145, 1330);
	append(i, 1280, 1348);
	append(i, 1234, 1377);
	append(i, 1269, 1373);
	append(i, 1152, 1423);
	append(i, 1154, 1397);
	append(i, 1148, 1399);
	append(i, 1291, 3122);
	append(i, 1232, 1450);
	append(i, 1288, 1442);
	append(i, 1504, 3340);
	append(i, 1343, 3179);
}
}

void ATM_ObjectLists::Init7()
{
	int i = 7;
	{
	append(i, 1188, 2190);
	append(i, 1186, 2194);
	append(i, 1000, 2178);
	append(i, 961, 2256);
	append(i, 988, 3273);
	append(i, 1000, 3274);
	append(i, 988, 3278);
	append(i, 991, 3283);
	append(i, 355, 2161);
	append(i, 350, 2158);
	append(i, 347, 2161);
	append(i, 351, 2165);
	append(i, 425, 2135);
	append(i, 431, 2139);
	append(i, 426, 2140);
	append(i, 613, 1720);
	append(i, 651, 1782);
}
}

void ATM_ObjectLists::Init8()
{
	int i = 8;
	{
	append(i, 1922, 1727);
	append(i, 1894, 1741);
	append(i, 1884, 1750);
	append(i, 1876, 1744);
	append(i, 1877, 1750);
	append(i, 1954, 1758);
	append(i, 1908, 1969);
	append(i, 1904, 1974);
	append(i, 1883, 1980);
	append(i, 1876, 2004);
	append(i, 1785, 2126);
	append(i, 1816, 2188);
	append(i, 1782, 2219);
	append(i, 1919, 2176);
	append(i, 2025, 2283);
	append(i, 2044, 2313);
	append(i, 1894, 2372);
	append(i, 1991, 2369);
	append(i, 1963, 2387);
	append(i, 1810, 2384);
	append(i, 2260, 2884);
	append(i, 2250, 2877);
	append(i, 1088, 1508);
	append(i, 1087, 1553);
	append(i, 1269, 1707);
	append(i, 1274, 1712);
	append(i, 1273, 1703);
	append(i, 1277, 1707);
	append(i, 1374, 1672);
	append(i, 1380, 1668);
	append(i, 1434, 1611);
	append(i, 1432, 1618);
	append(i, 1427, 1619);
	append(i, 1427, 1612);
	append(i, 1375, 1664);
	append(i, 1244, 1570);
	append(i, 1320, 1589);
	append(i, 649, 1760);
	append(i, 646, 1759);
	append(i, 649, 1753);
	append(i, 491, 2092);
	append(i, 500, 2084);
	append(i, 504, 2095);
	append(i, 408, 2258);
	append(i, 419, 2258);
	append(i, 1169, 2549);
	append(i, 1174, 2564);
	append(i, 1159, 2546);
	append(i, 1160, 2551);
	append(i, 1154, 2568);
	append(i, 801, 2966);
	append(i, 987, 3065);
	append(i, 1017, 3101);
	append(i, 966, 3106);
	append(i, 959, 3116);
	append(i, 973, 3107);
	append(i, 988, 3169);
	append(i, 749, 3212);
	append(i, 745, 3235);
	append(i, 754, 3249);
	append(i, 795, 3290);
	append(i, 921, 3301);
	append(i, 930, 3296);
	append(i, 1164, 3196);
	append(i, 1058, 3266);
	append(i, 1066, 3272);
	append(i, 1068, 3262);
	append(i, 1084, 3251);
	append(i, 1095, 3258);
	append(i, 1894, 2108);
	append(i, 1779, 2258);
	append(i, 1831, 2330);
	append(i, 1861, 2374);
	append(i, 1946, 2361);
	append(i, 1947, 2342);
	append(i, 2039, 2253);
	append(i, 1794, 2147);
	append(i, 1785, 2134);
	append(i, 1883, 1980);
	append(i, 1835, 2006);
	append(i, 1231, 1606);
	append(i, 1297, 1580);
	append(i, 1343, 1494);
	append(i, 1132, 3054);
	append(i, 605, 1693);
	append(i, 714, 1873);
	append(i, 723, 1887);
	append(i, 720, 1892);
	append(i, 754, 1897);
	append(i, 866, 1874);
	append(i, 870, 1872);
	append(i, 1352, 1535);
	append(i, 1812, 2193);
	append(i, 1812, 2181);
	append(i, 1787, 2140);
	append(i, 2025, 2274);
	append(i, 1971, 2359);
	append(i, 1702, 2421);
	append(i, 930, 3267);
	append(i, 927, 3273);
	append(i, 908, 3260);
	append(i, 782, 3092);
	append(i, 760, 3053);
	append(i, 857, 3099);
	append(i, 891, 3075);
	append(i, 920, 3048);
	append(i, 925, 3041);
	append(i, 1348, 2470);
	append(i, 1351, 2474);
	append(i, 1348, 2391);
	append(i, 1354, 2397);
	append(i, 1197, 2190);
	append(i, 379, 2117);
	append(i, 1157, 2057);
	append(i, 1165, 2058);
	append(i, 1151, 2052);
	append(i, 451, 2209);
	append(i, 980, 3267);
	append(i, 980, 3271);
	append(i, 1207, 2842);
}
}

void ATM_ObjectLists::Init9()
{
	int i = 9;
	{
	append(i, 2068, 1121);
	append(i, 2060, 1148);
	append(i, 2065, 1151);
	append(i, 2043, 1192);
	append(i, 2036, 1205);
	append(i, 2053, 1231);
	append(i, 2033, 1221);
	append(i, 2036, 1226);
	append(i, 2029, 1219);
	append(i, 2029, 1224);
	append(i, 2033, 1253);
	append(i, 2021, 1269);
	append(i, 2007, 1245);
	append(i, 2013, 1247);
	append(i, 2000, 1237);
	append(i, 2026, 1211);
	append(i, 2015, 1209);
	append(i, 2019, 1212);
	append(i, 2006, 1221);
	append(i, 2009, 1221);
	append(i, 1987, 1223);
	append(i, 1991, 1227);
	append(i, 1984, 1229);
	append(i, 1986, 1259);
	append(i, 1937, 1308);
	append(i, 1943, 1310);
	append(i, 1913, 1316);
	append(i, 1926, 1317);
	append(i, 1897, 1295);
	append(i, 1900, 1300);
	append(i, 1897, 1298);
	append(i, 2094, 1213);
	append(i, 2015, 1212);
}
}

void ATM_ObjectLists::Init10()
{
	int i = 10;
	{
	append(i, 3046, 1048);
	append(i, 3045, 1052);
	append(i, 3051, 1053);
	append(i, 3039, 1054);
	append(i, 3043, 1058);
	append(i, 2627, 1428);
	append(i, 2651, 1427);
	append(i, 2687, 1474);
	append(i, 2719, 1476);
	append(i, 2684, 1524);
	append(i, 2687, 1524);
	append(i, 2642, 1016);
	append(i, 2645, 1014);
	append(i, 2636, 968);
	append(i, 2637, 971);
	append(i, 2636, 974);
	append(i, 2601, 979);
	append(i, 2604, 973);
	append(i, 2600, 974);
	append(i, 2597, 973);
	append(i, 2596, 978);
	append(i, 2590, 969);
	append(i, 2591, 964);
	append(i, 2589, 945);
	append(i, 2585, 951);
	append(i, 2010, 902);
	append(i, 2925, 1263);
	append(i, 2932, 1254);
	append(i, 2967, 1240);
	append(i, 2958, 1243);
	append(i, 2954, 1236);
	append(i, 2940, 1258);
	append(i, 2948, 894);
	append(i, 2945, 896);
	append(i, 2948, 898);
	append(i, 2925, 849);
	append(i, 2924, 852);
	append(i, 2596, 1014);
	append(i, 2644, 1028);
	append(i, 2643, 1032);
	append(i, 2648, 1030);
	append(i, 2653, 1464);
	append(i, 2700, 1492);
	append(i, 1810, 1604);
	append(i, 1814, 1606);
	append(i, 1776, 1163);
	append(i, 1967, 1023);
	append(i, 1972, 1022);
	append(i, 1977, 1020);
	append(i, 1982, 1016);
	append(i, 1919, 998);
	append(i, 1923, 1001);
	append(i, 1917, 1005);
	append(i, 2075, 743);
	append(i, 2078, 744);
	append(i, 2075, 746);
	append(i, 1849, 1192);
	append(i, 1853, 1190);
	append(i, 1852, 1193);
	append(i, 2068, 1091);
	append(i, 2066, 1093);
	append(i, 2098, 1091);
	append(i, 2097, 1096);
	append(i, 2114, 1104);
	append(i, 2112, 1107);
	append(i, 2156, 1126);
	append(i, 2162, 1128);
	append(i, 2156, 1134);
	append(i, 2119, 1181);
	append(i, 2117, 1185);
	append(i, 2102, 1179);
	append(i, 1823, 1357);
	append(i, 1828, 1356);
	append(i, 1820, 1362);
	append(i, 1825, 1360);
}
}

void ATM_ObjectLists::Init11()
{
	int i = 11;
	{
		append(i, 1778, 2781);
		append(i, 1963, 2616);
		append(i, 2266, 2702);
		append(i, 2078, 2931);
		append(i, 2063, 2951);
		append(i, 2083, 2979);
		append(i, 2158, 2929);
		append(i, 2210, 2887);
		append(i, 2252, 2852);
		append(i, 2362, 2919);
		append(i, 2401, 2895);
		append(i, 2427, 2865);
		append(i, 2520, 2944);
		append(i, 2405, 3025);
		append(i, 2378, 3047);
		append(i, 2402, 3052);
		append(i, 2417, 3058);
		append(i, 2375, 3150);
		append(i, 2387, 3183);
		append(i, 2283, 3149);
		append(i, 2329, 3108);
		append(i, 2334, 3109);
		append(i, 2287, 3075);
		append(i, 2329, 3067);
		append(i, 2198, 3118);
		append(i, 1894, 2945);
		append(i, 1913, 2962);
		append(i, 1927, 2929);
		append(i, 1919, 2914);
		append(i, 1909, 2874);
		append(i, 1889, 2880);
		append(i, 1852, 2898);
		append(i, 1875, 2853);
		append(i, 1811, 2820);
		append(i, 1834, 2819);
		append(i, 1805, 2786);
		append(i, 1780, 2776);
		append(i, 1778, 2781);
		append(i, 1781, 2787);
		append(i, 1763, 2736);
		append(i, 1763, 2685);
		append(i, 1781, 2669);
		append(i, 1796, 2647);
		append(i, 1807, 2658);
		append(i, 1792, 2608);
		append(i, 1742, 2500);
		append(i, 1763, 2506);
		append(i, 1812, 2538);
		append(i, 1849, 2557);
		append(i, 1763, 2439);
		append(i, 1768, 2352);
		append(i, 1812, 2443);
		append(i, 1850, 2435);
		append(i, 1889, 2521);
		append(i, 1924, 2520);
		append(i, 1963, 2478);
		append(i, 1991, 2511);
		append(i, 2097, 2475);
		append(i, 2108, 2479);
		append(i, 2059, 2509);
		append(i, 1953, 2407);
		append(i, 1877, 2402);
		append(i, 2082, 2394);
		append(i, 2132, 2410);
		append(i, 2162, 2386);
		append(i, 2158, 2377);
		append(i, 2126, 2376);
		append(i, 1828, 2521);
		append(i, 1905, 2517);
	}
}

void ATM_ObjectLists::append(int i, int x, int y)
{
	if (i >= _objectListsNumber)
	{
		throw"Lists Full";
	}
	_objList[i].append(x, y);
}

void ATM_ObjectLists::reAppendFlag()
{
	_objFlag.clear();

	for (int ii = 0; ii < _collectionStateFlag.row(); ii++)
	{
		_objFlag.append(_collectionStateFlag[0][ii], _collectionStateFlag[1][ii]);
	}
}

void ATM_ObjectLists::reDeleteFlag()
{
}


