#include "GenshinImpact_AutoMap.h"

GenshinImpact_AutoMap::GenshinImpact_AutoMap(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	//�����ޱ߿�
	//setWindowFlags(Qt::CustomizeWindowHint);
	setWindowFlags(Qt::FramelessWindowHint);
	
}

void GenshinImpact_AutoMap::doubleClickExit()
{

}

bool GenshinImpact_AutoMap::eventFilter(QObject * watched, QEvent * event)
{
	return false;
}

void GenshinImpact_AutoMap::EventClickMove()
{
}
