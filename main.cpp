#include "ig_MainWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	IG_MainWidget widget1;
	widget1.show();
//	IG_MainWidget widget2;
//	widget2.show();

	return a.exec();
}
