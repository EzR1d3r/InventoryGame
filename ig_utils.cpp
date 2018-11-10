#include <QString>
#include <QCoreApplication>
#include <QDir>
#include "ig_utils.h"
#define DEBUG 1

using namespace IG_Utils;

QString IG_Utils::absPath(QString Path)
{
	QDir app_dir = QDir(QCoreApplication::applicationDirPath());
#if DEBUG
	QString absPath = app_dir.absolutePath()  + "/../.." + Path;
#else
	QString absPath = app_dir.absolutePath() + Path;
#endif

	return absPath;
}
