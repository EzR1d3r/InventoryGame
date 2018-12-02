#include <QString>
#include <QCoreApplication>
#include <QDir>
#include "ig_utils.h"
#define DEBUG 1

using namespace IG_Utils;

QString IG_Utils::absPathFromLocal(QString localPath)
{
	QDir app_dir = QDir(QCoreApplication::applicationDirPath());
#if DEBUG
	QString absPath = app_dir.absolutePath()  + "/../.." + localPath;
#else
	QString absPath = app_dir.absolutePath() + localPath;
#endif

	return absPath;
}
