#include "ig_MainWidget.h"
#include <QApplication>
#include "ig_pyextension.h"

int main(int argc, char *argv[])
{
	qDebug() << "====================";
	IG_PyCaller py_caller;
	py_caller.setPyModule("items_interaction");
	int res = py_caller.callFunc("multi", 1, 1);
	qDebug() << "res = " << res;


//	QApplication a(argc, argv);
//	IG_MainWidget widget1;
//	widget1.show();
//	IG_MainWidget widget2;
//	widget2.show();

//	return a.exec();
	return 0;
}
