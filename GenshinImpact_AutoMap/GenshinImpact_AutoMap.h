#pragma once
//#include <qfont.h>
#include <qfontdatabase.h>
#include <qtimer.h>
#include <qpainter.h>
#include <QMouseEvent>
#include <QtWidgets/QMainWindow>
#include "ui_GenshinImpact_AutoMap.h"
#include "AutomaticTrackingMap.h"
#include "QtClassMyHotKeyObject.h"
#include "QtWidgetsClassMySelectGenshinImpactHandle.h"

//最后生成版要删掉
#include <QDebug>

class GenshinImpact_AutoMap : public QMainWindow
{
    Q_OBJECT

public:
    GenshinImpact_AutoMap(QWidget *parent = Q_NULLPTR);
	~GenshinImpact_AutoMap();

private:
    Ui::GenshinImpact_AutoMapClass ui;

private:
	//自动追踪地图
	ATmap map;
	//地图鼠标指针图标
	QCursor *myCursor;
	//自动模式切换的全局热键
	QtClassMyHotKeyObject *hotKeyAutoMode;
	//添加删除自定义标记的全局热键
	QtClassMyHotKeyObject *hotKeyAddFlag;
	//激活反激活空荧酒馆切换的全局热键
	QtClassMyHotKeyObject *hotKeyActivationKongYingJiuGuan;
	//弹出选择原神窗口句柄窗口的全局热键
	QtClassMyHotKeyObject *hotKeyActivationSelectGenshinImpact;
	//弹出选择原神窗口句柄窗口的实例
	QtWidgetsClassMySelectGenshinImpactHandle *widgetsSelectGI = nullptr;

	//使用前需要对地图进行初始化
	void mapInit();
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void paintEvent(QPaintEvent *event);
private:
	//更新UID文本
	void displayUID(int uid);
private:
	//实现该帧率所需要的延时
	int Fps_ms = 42;//ms
	//画面更新，前后端识别更新定时器，实现帧率
	QTimer *mapMessageLoopTimer;
	//展开UI按键触发后的显示时长定时器
	QTimer *uiObjListSleepTimer;
private slots:
	//开始运行地图的绘制和识别更新
	void runMap();
	//前端绘制更新
	void updataFrontEnd();
	//后端识别更新
	void updataBackEnd();

	//切换自动或非自动模式
	void setAutoMode();
	//在当前位置添加或删除自定义标记
	void setAddFlag();
	//激活或反激活空荧酒馆客户端
	void setActivationKongYingJiuGuan();
	//弹出选择原神窗口句柄的窗口
	void setActivationSelectGenshinImpact();

	//展开显示UI面板
	void setUIObjListShow();
	//隐藏UI面板
	void setUIObjListHide();
	//UI面板每个按钮的点击触发
	void setUIObjListToMapData();

	//接收从选择原神窗口选择到的原神窗口句柄
	void getGenshinImpactWndHandleFromWidgets(HWND giHandle);
signals:
	//地图运行信号
	void mapUpdata();
	//前端运行信号
	void mapUpdataFrontEnd();
	//后端运行信号
	void mapUpdataBackEnd();

};
