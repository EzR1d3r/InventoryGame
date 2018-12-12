#include <algorithm>
#include "ig_databasemanager.h"
#include "ig_inventorytable.h"
#include "ig_slot.h"


IG_DataBaseManager::IG_DataBaseManager()
{
	__ID = __counter;
	__counter +=1;
}
int IG_DataBaseManager::__counter = 0;

void IG_DataBaseManager::loadAllItemsTo(IG_InventoryTable *pTable)
{
	//читаем данные из таблицы итемов
	QString query = "SELECT * FROM states_data ";
	QSqlQuery SqlQuery = QSqlQuery( __data_base );
	SqlQuery.exec( query );

	QSqlRecord rec = SqlQuery.record();

	//определяем индексы колонок
	int item_idx_idx	= rec.indexOf("item_idx");
	int state_idx		= rec.indexOf("state");
	int img_path_idx	= rec.indexOf("img_path");
	int snd_path_idx	= rec.indexOf("snd_path");

	//составляем QHash структур (параметры итема), ключ - индекс итема
	QHash<uint, item_params> all_items_params;

	while (SqlQuery.next())
	{
		rec = SqlQuery.record();
		item_params params;

		params.__idx =			  rec.value( item_idx_idx ).toUInt();
		params.__states.insert	( rec.value( state_idx ).toInt(), rec.value( img_path_idx ).toString()  );
		params.__snds.insert	( rec.value( state_idx ).toInt(), rec.value( snd_path_idx ).toString()  );

		if( all_items_params.contains( params.__idx ) )
			{
				all_items_params[params.__idx].__states.insert	(rec.value( state_idx ).toInt(),
																 rec.value( img_path_idx ).toString());

				all_items_params[params.__idx].__snds.insert	(rec.value( state_idx ).toInt(),
																 rec.value( snd_path_idx ).toString());
			}
		else
			{
				all_items_params.insert( params.__idx, params );
			}
	}

	//читаем данные из таблицы типов и добавляем в QHash параметров
	query = "SELECT * FROM interact_type ";
	SqlQuery.exec( query );

	while (SqlQuery.next())
	{
		rec = SqlQuery.record();
		int item_idx_idx	= rec.indexOf("item_idx");
		int inct_type_idx	= rec.indexOf("inct_type");

		all_items_params[ rec.value( item_idx_idx ).toUInt() ].__interact_type = rec.value( inct_type_idx ).toInt();
	}

	//проходим по хешу и создаем итемы, создаём слоты (QTableWidgetItem), заполняем ими таблицу
	for (auto params:all_items_params)
	{
		//проходим по листу стейтов итема, находим максимальный, считаем что это текущий стейт
		QList<int> all_states = params.__states.keys();
		auto itr_max_state = std::max_element(all_states.begin(), all_states.end());
		params.__max_state = *itr_max_state;
		params.__state = params.__max_state;

		//создаём итем со всеми параметрами
		IG_Item item( params );

		if (!params.__idx) continue;
		IG_Slot * pSlot = new IG_Slot ( pTable );
		pSlot->addItem( item );
		pSlot->setInfinite(true);
		pTable->setRowCount( pTable->rowCount() + 1 );
		pTable->setItem( pTable->rowCount() - 1, 0, pSlot );
	}

}

void IG_DataBaseManager::connectSQLiteDB(QString name)
{
	QString id = QString("connection_SQLiteDB_ID%1").arg(__ID);
	__data_base = QSqlDatabase::addDatabase("QSQLITE", id);
	__data_base.setDatabaseName(name);

	if (!__data_base.open())
	{
		qDebug() << "Error open database: " << name;
		return;
	}
}

void IG_DataBaseManager::disconnectSQLiteDB()
{
	__data_base.close();
}

