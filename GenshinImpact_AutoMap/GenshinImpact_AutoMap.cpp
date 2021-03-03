#include "GenshinImpact_AutoMap.h"

GenshinImpact_AutoMap::GenshinImpact_AutoMap(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	//����UI��尴ť���ɼ�
	ui.UIObjFlagButton->setVisible(false);
	ui.UIObjList0Button->setVisible(false);
	ui.UIObjList1Button->setVisible(false);
	ui.UIObjList2Button->setVisible(false);
	ui.UIObjList999Button->setVisible(false);
	//ΪUI��尴ť�󶨴����¼������¼����л���ť��Ӧ��ͼ��Դͼ�����ʾ����
	connect(ui.UIObjFlagButton, SIGNAL(clicked()), this, SLOT(setUIObjListToMapData()));
	connect(ui.UIObjList0Button, SIGNAL(clicked()), this, SLOT(setUIObjListToMapData()));
	connect(ui.UIObjList1Button, SIGNAL(clicked()), this, SLOT(setUIObjListToMapData()));
	connect(ui.UIObjList2Button, SIGNAL(clicked()), this, SLOT(setUIObjListToMapData()));
	connect(ui.UIObjList999Button, SIGNAL(clicked()), this, SLOT(setUIObjListToMapData()));

	//����ˢ�¶�ʱ��
	mapMessageLoopTimer = new QTimer(this);
	mapMessageLoopTimer->start(Fps_ms);//1000/30=33.3,1000/24=42������1000ms����֡��
	connect(mapMessageLoopTimer, SIGNAL(timeout()), this, SLOT(runMap()));
	//����UIObjList���ʱ��
	uiObjListSleepTimer = new QTimer(this);
	uiObjListSleepTimer->setSingleShot(true);
	connect(uiObjListSleepTimer, SIGNAL(timeout()), this, SLOT(setUIObjListHide()));

	// Alt+T
	// Alt+F
	// Alt+M
	// Alt+F8

	//���ȫ�ֿ�ݼ�
	hotKeyAutoMode = new QtClassMyHotKeyObject("Alt+T", this);
	connect(hotKeyAutoMode, SIGNAL(Activated()), this, SLOT(setAutoMode()));
	hotKeyAddFlag = new QtClassMyHotKeyObject("Alt+F", this);
	connect(hotKeyAddFlag, SIGNAL(Activated()), this, SLOT(setAddFlag()));
	hotKeyActivationKongYingJiuGuan = new QtClassMyHotKeyObject("Alt+M", this);
	connect(hotKeyActivationKongYingJiuGuan, SIGNAL(Activated()), this, SLOT(setActivationKongYingJiuGuan()));
	hotKeyActivationSelectGenshinImpact = new QtClassMyHotKeyObject("Alt+F8", this);
	connect(hotKeyActivationSelectGenshinImpact, SIGNAL(Activated()), this, SLOT(setActivationSelectGenshinImpact()));
	//UI��ʾ��ť���¼���
	connect(ui.UIButton, SIGNAL(clicked()), this, SLOT(setUIObjListShow()));
	//˫�����ɰ�ť�˳�����
	connect(ui.ExitButton, SIGNAL(mouseDoubleClickExitExe()), this, SLOT(close()));
	//��ǰ����¼�
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

	//�Ե�ͼ���г�ʼ��
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
	//��ͼ��ʵ����ȡ���ھ������������ƣ�ʵ���Զ����ö�����С��
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
	//��¼�ϴ�����ʱUID���Լ����ظ�����
	//�������ò��ϣ�����UID���Ѿ�ȷ���˱�Ȼ���µ�
	// TODO: �д�����
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

	//��ʱ��Ҫ�ǳ����ں�����ݸ�����
	//�����ǰ֡ʱ���ں�����ݸ���δ���
	//����һ֡��������������ǰ�˻�����л���
	if (isUpdataEnd)
	{
		isUpdataEnd = false;
		emit this->mapUpdataFrontEnd();
		emit this->mapUpdataBackEnd();
		//���ݸ������
		isUpdataEnd = true;
		//������һ֡
		mapMessageLoopTimer->start(Fps_ms);
	}
	else
	{
		//��һ֡����δ��ɣ���һ֡����
		mapMessageLoopTimer->start(Fps_ms);
	}
}

void GenshinImpact_AutoMap::updataFrontEnd()
{
	//���»���ͼ��
	map.FrontEndUpdata();
	//����UID������
	displayUID(map.getUID());
	//���Ƶ�����
	update();
}

void GenshinImpact_AutoMap::updataBackEnd()
{
	//ִ�к�˵�ͼ��ʶ��Ȳ�������ʶ�𵽵���Ϸ����
	map.BackEndUpdata();
}

void GenshinImpact_AutoMap::setAutoMode()
{
	//�л��Զ�����Զ�ģʽ
	map.setAutoMode();
	//���ݲ�ͬ��ģʽ���ò�ͬ��ͼ��
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
	//�ڵ�ǰ��ͼ��ʾ����λ����ӱ�ǻ���ɾ�����б��
	map.setAddFlagOnPos();
}

void GenshinImpact_AutoMap::setActivationKongYingJiuGuan()
{
	//��������ӫ�ƹݵĴ��ͼ����ģʽ
	map.setKongYingJiuGuanState();
}

void GenshinImpact_AutoMap::setActivationSelectGenshinImpact()
{
	//��ʾԭ�񴰿�ѡ�񴰿�
	//���δ�������ʹ�����ʾ������ֱ����ʾ
	//��Ϊ�ô���ʵ���ڴ�����û������
	if (widgetsSelectGI == nullptr)
	{
		widgetsSelectGI = new QtWidgetsClassMySelectGenshinImpactHandle();
		//���¼��������Ӵ����򸸴��ڴ���ѡ���ԭ�񴰿ھ��
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
	//UI�����ʾ�ǣ�����UI��ť��ͼ��
	ui.UIButton->setIcon(QIcon(":/IconUI/resource/UI1.ico"));

	//��ʾÿһ��UI���İ�ť
	ui.UIObjFlagButton->setVisible(true);
	ui.UIObjList0Button->setVisible(true);
	ui.UIObjList1Button->setVisible(true);
	ui.UIObjList2Button->setVisible(true);
	ui.UIObjList999Button->setVisible(true);

	//������ʾʱ��
	//��ʱ�����������
	//δ����ʱ�ٴΰ���UI��ť��ˢ�����ص���ʱ
	uiObjListSleepTimer->start(2000);
}

void GenshinImpact_AutoMap::setUIObjListHide()
{
	//����ÿһ��UI���İ�ť
	ui.UIObjFlagButton->setVisible(false);
	ui.UIObjList0Button->setVisible(false);
	ui.UIObjList1Button->setVisible(false);
	ui.UIObjList2Button->setVisible(false);
	ui.UIObjList999Button->setVisible(false);

	//��ԭUI��尴ťͼ��
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
	//��ȡ��ǰ����¼���Դ��ť����
	//������Դ���󴥷��ö���Ĳ���
	//���л���Ӧ������Դ�ڵ�ͼ�ϵ���ʾ������
	QPushButton *btn = qobject_cast<QPushButton*>(sender());
	if (btn == ui.UIObjFlagButton) 
	{
		//�Զ������л��Ƕ��صģ�ʹ��һ�������ķ���
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
		// TODO: ��Ҫ���ģ���ΪҪ��ӱ����Դ
	}
}

void GenshinImpact_AutoMap::getGenshinImpactWndHandleFromWidgets(HWND giHandle)
{
	//��ѡ����ԭ����Ϸ���ڵľ�����ݸ���ͼ�࣬�ɵ�ͼ����
	map.setGenshinImpactWndHandle(giHandle);
}


