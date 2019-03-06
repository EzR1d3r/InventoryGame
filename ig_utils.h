#pragma once
#include <QVector>

//Локальные пути
#define DB_PATH "/sqlite/inv_game.sqlite"
#define PY_SCRIPTS_PATH "/pyscripts"
#define IMAGES_PATH "/images"
#define SOUNDS_PATH "/snds"
#define IMAGES_SRC_PATH ":/src/images"
#define SOUNDS_SRC_PATH ":/src/snds"

//Модули питона и их функции
#define PY_ITEMS_INTERACTION	"items_interaction"
#define PY_FUNC_INTERACT		"interact"
#define PY_FUNC_CHECK_STACKABLE	"check_stackable"

// признак дебаг-сборки, по нему составляются абсолютные пути -
// либо папка проекта либо корень сборки

#ifndef QT_NO_DEBUG //release
	#define DEBUG
#endif

namespace IG_Utils
{
	QString absPathFromLocal(QString localPath);
	QString findFile ( QVector<QString> dirs, QString name );
	QString findImg(QString name);
	QString findSnd(QString name);
}
