#include "ATM_Modules.h"

double ATM_Modules::dis(double x1, double x2)
{
	return sqrt(x1*x1+x2*x2);
}

double ATM_Modules::dis(Point2d p)
{
	return dis(p.x,p.y);
}

double ATM_Modules::dis(Point2d p1, Point2d p2)
{
	return dis(p1.x-p2.x,p1.y-p2.y);
}

bool ATM_Modules::isContains(Rect & r, Point2d & p)
{
	if (p.x<r.x || p.x>(r.x + r.width) || p.y<r.y || p.y>(r.y + r.height))
	{
		return false;
	}
	else
	{
		return true;
	}
	return false;
}
