#pragma once
#include <QTimer>
#include <QByteArray>
#include <QLocalSocket>
#include <QApplication>
#include <QSharedMemory>

class QtSingleApplication : public QApplication
{
	Q_OBJECT
public:
	QtSingleApplication(int &argc, char *argv[], const QString uniqueKey);
	bool isRunning();
	bool sendMessage(const QString &message);
public slots:
	void checkForMessage();
signals:
	void messageAvailable(QString message);
private:
	bool _isRunning;
	QSharedMemory sharedMemory;
};
