#include "GenshinImpact_AutoMap.h"
#include <QtWidgets/QApplication>
#include "QtSingleApplication.h"

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
	QtSingleApplication a(argc, argv, "TianLi Map UniqueKey");
	if (a.isRunning())
	{
		a.sendMessage("TianLi Map Is Running");
		return 0;
	}

    GenshinImpact_AutoMap w;
    w.show();
    return a.exec();
}
