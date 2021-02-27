#include "GenshinImpact_AutoMap.h"

GenshinImpact_AutoMap::GenshinImpact_AutoMap(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	ui.UIObjFlagButton->setVisible(false);
	ui.UIObjList0Button->setVisible(false);
	ui.UIObjList1Button->setVisible(false);
	ui.UIObjList2Button->setVisible(false);
	ui.UIObjList999Button->setVisible(false);
	connect(ui.UIObjFlagButton, SIGNAL(clicked()), this, SLOT(setUIObjListToMapData()));
	connect(ui.UIObjList0Button, SIGNAL(clicked()), this, SLOT(setUIObjListToMapData()));
	connect(ui.UIObjList1Button, SIGNAL(clicked()), this, SLOT(setUIObjListToMapData()));
	connect(ui.UIObjList2Button, SIGNAL(clicked()), this, SLOT(setUIObjListToMapData()));
	connect(ui.UIObjList999Button, SIGNAL(clicked()), this, SLOT(setUIObjListToMapData()));


	//����ˢ�¶�ʱ��
	mapMessageLoopTimer = new QTimer(this);
	mapMessageLoopTimer->start(Fps);//1000/30=33.3,1000/24=42
	connect(mapMessageLoopTimer, SIGNAL(timeout()), this, SLOT(runMap()));
	//����UIObjList���ʱ��
	uiObjListSleepTimer = new QTimer(this);
	uiObjListSleepTimer->setSingleShot(true);
	connect(uiObjListSleepTimer, SIGNAL(timeout()), this, SLOT(setUIObjListHide()));

	//���ȫ�ֿ�ݼ�
	hotKeyAutoMode = new QtClassMyHotKeyObject("Alt+T", this);
	connect(hotKeyAutoMode, SIGNAL(Activated()), this, SLOT(setAutoMode()));
	//hotKeyAddFlag = new QtClassMyHotKeyObject("Alt+F", this);
	//connect(hotKeyAddFlag, SIGNAL(Activated()), this, SLOT(setAddFlag()));
	//hotKeyActivationKongYingJiuGuan = new QtClassMyHotKeyObject("Alt+M", this);
	//connect(hotKeyActivationKongYingJiuGuan, SIGNAL(Activated()), this, SLOT(setActivationKongYingJiuGuan()));
	//hotKeyActivationSelectGenshinImpact = new QtClassMyHotKeyObject("Alt+F8", this);
	//connect(hotKeyActivationSelectGenshinImpact, SIGNAL(Activated()), this, SLOT(setActivationSelectGenshinImpact()));

	connect(ui.UIButton, SIGNAL(clicked()), this, SLOT(setUIObjListShow()));
	connect(ui.ExitButton, SIGNAL(mouseDoubleClickExitExe()), this, SLOT(close()));
	connect(this, &GenshinImpact_AutoMap::mapUpdataFrontEnd, this, &GenshinImpact_AutoMap::updataFrontEnd);
	connect(this, &GenshinImpact_AutoMap::mapUpdataBackEnd, this, &GenshinImpact_AutoMap::updataBackEnd);

	//�������ָ��
	myCursor = new QCursor(QPixmap(":/CurMain/resource/GenshinImpactCur.cur"), 2, 2);
	this->setCursor(*myCursor);

	//���ð�͸���ޱ߿򴰿�
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.MainView->setAttribute(Qt::WA_TranslucentBackground);

	//����UID����
	int UIDFontId = QFontDatabase::addApplicationFont(":/UIDFont/resource/UIDFont.ttf");
	QString UIDFontName = QFontDatabase::applicationFontFamilies(UIDFontId).at(0);
	QFont UIDFont(UIDFontName,12);
	ui.UIDLabel->setFont(UIDFont);

	mapInit();
}

GenshinImpact_AutoMap::~GenshinImpact_AutoMap()
{
	delete myCursor;
	delete hotKeyAutoMode;
	delete hotKeyAddFlag;
	delete hotKeyActivationKongYingJiuGuan;
	delete hotKeyActivationSelectGenshinImpact;
	delete widgetsSelectGI;
}

void GenshinImpact_AutoMap::mapInit()
{
	map.Init((HWND)this->winId());
}

void GenshinImpact_AutoMap::mouseMoveEvent(QMouseEvent * event)
{
	if (map.MET.bLCD)
	{
		map.setMoveMapMovePos(event->x(), event->y());
		update();
	}
	if (map.MET.bMCD)
	{
		map.setOffsetMovePos(event->globalX(), event->globalY());
		map.setWindowsPos();
	}
}

void GenshinImpact_AutoMap::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		// �������
		map.MET.bLCD = true;
		map.setMoveMapDownPos(event->x(), event->y());
	}
	if (event->button() == Qt::MidButton)
	{
		map.MET.bMCD = true;
		map.setOffsetDownPos(event->globalPos().x(), event->globalPos().y());
		map.CustomProcess(0);
	}
}

void GenshinImpact_AutoMap::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		// �������
		map.MET.bLCD = false;
	}
	if (event->button() == Qt::MidButton)
	{
		map.MET.bMCD = false;
	}
}

void GenshinImpact_AutoMap::mouseDoubleClickEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit this->setAutoMode();
	}
}

void GenshinImpact_AutoMap::wheelEvent(QWheelEvent * event)
{
	map.setScaleMapDelta(event->x(), event->y(), event->delta());
	update();
}

void GenshinImpact_AutoMap::paintEvent(QPaintEvent * event)
{
	//���û���Ϊ��ͼ
	QPainter painter(this);
	painter.drawImage(0, 0, map.MainImg);
}

void GenshinImpact_AutoMap::displayUID(int uid)
{
	static int uidTmp=0;
	if (uidTmp != uid)
	{
		ui.UIDLabel->setText(QString::QString("UID: %1").arg(uid));
		uidTmp = uid;
	}
}

void GenshinImpact_AutoMap::runMap()
{
	static bool isUpdataEnd = true;
	//�����Ƿ��ѽ���
	if (isUpdataEnd)
	{
		isUpdataEnd = false;
		emit this->mapUpdataFrontEnd();
		emit this->mapUpdataBackEnd();
		isUpdataEnd = true;
		//������һ֡
		mapMessageLoopTimer->start(Fps);
	}
	else
	{
		//������һ֡
		mapMessageLoopTimer->start(Fps);
	}


}

void GenshinImpact_AutoMap::updataFrontEnd()
{
	//���»���ͼ��
	map.FrontEndUpdata();
	displayUID(map.getUID());
	//���Ƶ�����
	update();
}

void GenshinImpact_AutoMap::updataBackEnd()
{
	map.BackEndUpdata();
}

void GenshinImpact_AutoMap::setAutoMode()
{
	map.setAutoMode();
	if (map.isAutoMode)
	{
		ui.UIButton->setIcon(QIcon(":/IconUI/resource/UI0.ico"));
	}
	else
	{
		ui.UIButton->setIcon(QIcon(":/IconUI/resource/UI.ico"));
	}

}

void GenshinImpact_AutoMap::setAddFlag()
{
	//map.zerosMinMap;
	map.setAddFlagOnPos();
}

void GenshinImpact_AutoMap::setActivationKongYingJiuGuan()
{
	map.setKongYingJiuGuanState();
}

void GenshinImpact_AutoMap::setActivationSelectGenshinImpact()
{
	if (widgetsSelectGI == nullptr)
	{
		widgetsSelectGI = new QtWidgetsClassMySelectGenshinImpactHandle();
		connect(widgetsSelectGI, SIGNAL(SendGenshinImpactWndHandleToATM(HWND)), this, SLOT(getGenshinImpactWndHandleFromWidgets(HWND)));
		widgetsSelectGI->show();

	}
	else
	{
		widgetsSelectGI->show();

	}
}

void GenshinImpact_AutoMap::setUIObjListShow()
{
	ui.UIButton->setIcon(QIcon(":/IconUI/resource/UI1.ico"));

	ui.UIObjFlagButton->setVisible(true);
	ui.UIObjList0Button->setVisible(true);
	ui.UIObjList1Button->setVisible(true);
	ui.UIObjList2Button->setVisible(true);
	ui.UIObjList999Button->setVisible(true);

	uiObjListSleepTimer->start(2000);


}

void GenshinImpact_AutoMap::setUIObjListHide()
{
	ui.UIObjFlagButton->setVisible(false);
	ui.UIObjList0Button->setVisible(false);
	ui.UIObjList1Button->setVisible(false);
	ui.UIObjList2Button->setVisible(false);
	ui.UIObjList999Button->setVisible(false);


	if (map.isAutoMode)
	{
		ui.UIButton->setIcon(QIcon(":/IconUI/resource/UI0.ico"));
	}
	else
	{
		ui.UIButton->setIcon(QIcon(":/IconUI/resource/UI.ico"));
	}
}

void GenshinImpact_AutoMap::setUIObjListToMapData()
{
	QPushButton *btn = qobject_cast<QPushButton*>(sender());
	if (btn == ui.UIObjFlagButton) 
	{
		map.setObjFlagIsShow();
	}
	if (btn == ui.UIObjList0Button) 
	{
		map.setObjIsShow(0);
	}
	if (btn == ui.UIObjList1Button) 
	{
		map.setObjIsShow(1);
	}
	if (btn == ui.UIObjList2Button) 
	{
		map.setObjIsShow(2);
	}
	if (btn == ui.UIObjList999Button)
	{
		map.setObjIsShow(3);
	}
}

void GenshinImpact_AutoMap::getGenshinImpactWndHandleFromWidgets(HWND giHandle)
{
	map.setGenshinImpactWndHandle(giHandle);
}


