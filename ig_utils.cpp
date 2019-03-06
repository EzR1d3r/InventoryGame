#include <QString>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include "ig_utils.h"
#include <QDebug>

using namespace IG_Utils;

QString IG_Utils::absPathFromLocal(QString localPath)
{
	QDir app_dir = QDir(QCoreApplication::applicationDirPath());
#ifdef DEBUG
	QString absPath = app_dir.absolutePath()  + "/../.." + localPath;
#else
	QString absPath = app_dir.absolutePath() + localPath;
#endif

	return absPath;
}

QString IG_Utils::findImg(QString name)
{
	QVector<QString> dirs {QString(IMAGES_SRC_PATH), IG_Utils::absPathFromLocal(IMAGES_PATH)};
	return IG_Utils::findFile( dirs, name );
}

QString IG_Utils::findSnd(QString name)
{
	QVector<QString> dirs {QString(SOUNDS_SRC_PATH), IG_Utils::absPathFromLocal(SOUNDS_PATH)};
	return IG_Utils::findFile( dirs, name );
}

QString IG_Utils::findFile(QVector<QString> dirs, QString name)
{
	for (auto dir:dirs)
	{
		QString full_path = QString("%1/%2").arg(dir).arg(name);
		if ( QFileInfo(full_path).exists() )
			return full_path;
	}
	return QString();
}
