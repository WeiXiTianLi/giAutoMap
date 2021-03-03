#include "GenshinImpact_AutoMap.h"

GenshinImpact_AutoMap::GenshinImpact_AutoMap(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	//设置UI面板按钮不可见
	ui.UIObjFlagButton->setVisible(false);
	ui.UIObjList0Button->setVisible(false);
	ui.UIObjList1Button->setVisible(false);
	ui.UIObjList2Button->setVisible(false);
	ui.UIObjList999Button->setVisible(false);
	//为UI面板按钮绑定触发事件，该事件将切换按钮对应地图资源图标的显示隐藏
	connect(ui.UIObjFlagButton, SIGNAL(clicked()), this, SLOT(setUIObjListToMapData()));
	connect(ui.UIObjList0Button, SIGNAL(clicked()), this, SLOT(setUIObjListToMapData()));
	connect(ui.UIObjList1Button, SIGNAL(clicked()), this, SLOT(setUIObjListToMapData()));
	connect(ui.UIObjList2Button, SIGNAL(clicked()), this, SLOT(setUIObjListToMapData()));
	connect(ui.UIObjList999Button, SIGNAL(clicked()), this, SLOT(setUIObjListToMapData()));

	//创建刷新定时器
	mapMessageLoopTimer = new QTimer(this);
	mapMessageLoopTimer->start(Fps_ms);//1000/30=33.3,1000/24=42，等于1000ms除以帧率
	connect(mapMessageLoopTimer, SIGNAL(timeout()), this, SLOT(runMap()));
	//创建UIObjList激活定时器
	uiObjListSleepTimer = new QTimer(this);
	uiObjListSleepTimer->setSingleShot(true);
	connect(uiObjListSleepTimer, SIGNAL(timeout()), this, SLOT(setUIObjListHide()));

	// Alt+T
	// Alt+F
	// Alt+M
	// Alt+F8

	//添加全局快捷键
	hotKeyAutoMode = new QtClassMyHotKeyObject("Alt+T", this);
	connect(hotKeyAutoMode, SIGNAL(Activated()), this, SLOT(setAutoMode()));
	hotKeyAddFlag = new QtClassMyHotKeyObject("Alt+F", this);
	connect(hotKeyAddFlag, SIGNAL(Activated()), this, SLOT(setAddFlag()));
	hotKeyActivationKongYingJiuGuan = new QtClassMyHotKeyObject("Alt+M", this);
	connect(hotKeyActivationKongYingJiuGuan, SIGNAL(Activated()), this, SLOT(setActivationKongYingJiuGuan()));
	hotKeyActivationSelectGenshinImpact = new QtClassMyHotKeyObject("Alt+F8", this);
	connect(hotKeyActivationSelectGenshinImpact, SIGNAL(Activated()), this, SLOT(setActivationSelectGenshinImpact()));
	//UI显示按钮的事件绑定
	connect(ui.UIButton, SIGNAL(clicked()), this, SLOT(setUIObjListShow()));
	//双击派蒙按钮退出程序
	connect(ui.ExitButton, SIGNAL(mouseDoubleClickExitExe()), this, SLOT(close()));
	//绑定前后端事件
	connect(this, &GenshinImpact_AutoMap::mapUpdataFrontEnd, this, &GenshinImpact_AutoMap::updataFrontEnd);
	connect(this, &GenshinImpact_AutoMap::mapUpdataBackEnd, this, &GenshinImpact_AutoMap::updataBackEnd);

	//设置鼠标指针
	myCursor = new QCursor(QPixmap(":/CurMain/resource/GenshinImpactCur.cur"), 2, 2);
	this->setCursor(*myCursor);

	//设置半透明无边框窗口
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.MainView->setAttribute(Qt::WA_TranslucentBackground);

	//设置UID字体
	int UIDFontId = QFontDatabase::addApplicationFont(":/UIDFont/resource/UIDFont.ttf");
	QString UIDFontName = QFontDatabase::applicationFontFamilies(UIDFontId).at(0);
	QFont UIDFont(UIDFontName,12);
	ui.UIDLabel->setFont(UIDFont);

	//对地图进行初始化
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
	//地图类实例获取窗口句柄，并对其控制，实现自动的置顶和最小化
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
		// 左键按下
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
		// 左键按下
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
	//设置画面为地图
	QPainter painter(this);
	painter.drawImage(0, 0, map.MainImg);
}

void GenshinImpact_AutoMap::displayUID(int uid)
{
	//记录上次设置时UID，以减少重复设置
	//但好像用不上？传递UID是已经确保了必然是新的
	// TODO: 有待更改
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

	//耗时主要是出现在后端数据更新上
	//如果当前帧时间内后端数据更新未完成
	//则下一帧继续处理，而不对前端画面进行绘制
	if (isUpdataEnd)
	{
		isUpdataEnd = false;
		emit this->mapUpdataFrontEnd();
		emit this->mapUpdataBackEnd();
		//数据更新完成
		isUpdataEnd = true;
		//启动下一帧
		mapMessageLoopTimer->start(Fps_ms);
	}
	else
	{
		//上一帧工作未完成，下一帧继续
		mapMessageLoopTimer->start(Fps_ms);
	}
}

void GenshinImpact_AutoMap::updataFrontEnd()
{
	//更新绘制图像
	map.FrontEndUpdata();
	//绘制UID到窗口
	displayUID(map.getUID());
	//绘制到窗口
	update();
}

void GenshinImpact_AutoMap::updataBackEnd()
{
	//执行后端的图像识别等操作更新识别到的游戏数据
	map.BackEndUpdata();
}

void GenshinImpact_AutoMap::setAutoMode()
{
	//切换自动或非自动模式
	map.setAutoMode();
	//根据不同的模式设置不同的图标
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
	//在当前地图显示中心位置添加标记或者删除已有标记
	map.setAddFlagOnPos();
}

void GenshinImpact_AutoMap::setActivationKongYingJiuGuan()
{
	//激活反激活空荧酒馆的大地图覆盖模式
	map.setKongYingJiuGuanState();
}

void GenshinImpact_AutoMap::setActivationSelectGenshinImpact()
{
	//显示原神窗口选择窗口
	//如果未创建过就创建显示，否则直接显示
	//因为该窗口实例在创建后并没有析构
	if (widgetsSelectGI == nullptr)
	{
		widgetsSelectGI = new QtWidgetsClassMySelectGenshinImpactHandle();
		//绑定事件用来从子窗口向父窗口传递选择的原神窗口句柄
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
	//UI面板显示是，更改UI按钮的图标
	ui.UIButton->setIcon(QIcon(":/IconUI/resource/UI1.ico"));

	//显示每一个UI面板的按钮
	ui.UIObjFlagButton->setVisible(true);
	ui.UIObjList0Button->setVisible(true);
	ui.UIObjList1Button->setVisible(true);
	ui.UIObjList2Button->setVisible(true);
	ui.UIObjList999Button->setVisible(true);

	//设置显示时间
	//超时触发隐藏面板
	//未触发时再次按下UI按钮则刷新隐藏倒计时
	uiObjListSleepTimer->start(2000);
}

void GenshinImpact_AutoMap::setUIObjListHide()
{
	//隐藏每一个UI面板的按钮
	ui.UIObjFlagButton->setVisible(false);
	ui.UIObjList0Button->setVisible(false);
	ui.UIObjList1Button->setVisible(false);
	ui.UIObjList2Button->setVisible(false);
	ui.UIObjList999Button->setVisible(false);

	//还原UI面板按钮图标
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
	//获取当前点击事件来源按钮对象
	//根据来源对象触发该对象的操作
	//即切换对应类型资源在地图上的显示或隐藏
	QPushButton *btn = qobject_cast<QPushButton*>(sender());
	if (btn == ui.UIObjFlagButton) 
	{
		//自定义标记切换是独特的，使用一个单独的方法
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
		// TODO: 需要更改，因为要添加别的资源
	}
}

void GenshinImpact_AutoMap::getGenshinImpactWndHandleFromWidgets(HWND giHandle)
{
	//将选定的原神游戏窗口的句柄传递给地图类，由地图处理
	map.setGenshinImpactWndHandle(giHandle);
}


