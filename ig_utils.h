#pragma once

//Локальные пути
#define DB_PATH "/sqlite/inv_game.sqlite"
#define PY_SCRIPTS_PATH "/pyscripts"

//Модули питона и их функции
#define PY_ITEMS_INTERACTION	"items_interaction"
#define PY_FUNC_INTERACT		"interact"

class QString;

namespace IG_Utils
{
	QString absPathFromLocal(QString localPath);
} // namespace IG_Utils
