#include "ig_MainWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	IG_MainWidget widget1;
	IG_MainWidget widget2;
	widget1.show();
	widget2.show();

	return a.exec();
}
