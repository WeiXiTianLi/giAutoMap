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

//������ɰ�Ҫɾ��
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
	//�Զ�׷�ٵ�ͼ
	ATmap map;
	//��ͼ���ָ��ͼ��
	QCursor *myCursor;
	//�Զ�ģʽ�л���ȫ���ȼ�
	QtClassMyHotKeyObject *hotKeyAutoMode;
	//���ɾ���Զ����ǵ�ȫ���ȼ�
	QtClassMyHotKeyObject *hotKeyAddFlag;
	//��������ӫ�ƹ��л���ȫ���ȼ�
	QtClassMyHotKeyObject *hotKeyActivationKongYingJiuGuan;
	//����ѡ��ԭ�񴰿ھ�����ڵ�ȫ���ȼ�
	QtClassMyHotKeyObject *hotKeyActivationSelectGenshinImpact;
	//����ѡ��ԭ�񴰿ھ�����ڵ�ʵ��
	QtWidgetsClassMySelectGenshinImpactHandle *widgetsSelectGI = nullptr;

	//ʹ��ǰ��Ҫ�Ե�ͼ���г�ʼ��
	void mapInit();
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void paintEvent(QPaintEvent *event);
private:
	//����UID�ı�
	void displayUID(int uid);
private:
	//ʵ�ָ�֡������Ҫ����ʱ
	int Fps_ms = 42;//ms
	//������£�ǰ���ʶ����¶�ʱ����ʵ��֡��
	QTimer *mapMessageLoopTimer;
	//չ��UI�������������ʾʱ����ʱ��
	QTimer *uiObjListSleepTimer;
private slots:
	//��ʼ���е�ͼ�Ļ��ƺ�ʶ�����
	void runMap();
	//ǰ�˻��Ƹ���
	void updataFrontEnd();
	//���ʶ�����
	void updataBackEnd();

	//�л��Զ�����Զ�ģʽ
	void setAutoMode();
	//�ڵ�ǰλ����ӻ�ɾ���Զ�����
	void setAddFlag();
	//����򷴼����ӫ�ƹݿͻ���
	void setActivationKongYingJiuGuan();
	//����ѡ��ԭ�񴰿ھ���Ĵ���
	void setActivationSelectGenshinImpact();

	//չ����ʾUI���
	void setUIObjListShow();
	//����UI���
	void setUIObjListHide();
	//UI���ÿ����ť�ĵ������
	void setUIObjListToMapData();

	//���մ�ѡ��ԭ�񴰿�ѡ�񵽵�ԭ�񴰿ھ��
	void getGenshinImpactWndHandleFromWidgets(HWND giHandle);
signals:
	//��ͼ�����ź�
	void mapUpdata();
	//ǰ�������ź�
	void mapUpdataFrontEnd();
	//��������ź�
	void mapUpdataBackEnd();

};
