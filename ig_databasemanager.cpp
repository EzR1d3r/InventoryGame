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

void IG_DataBaseManager::query_create_inventory_table(QString name)
{
	QString query = "CREATE TABLE IF NOT EXISTS %1 (	id INTEGER PRIMARY KEY AUTOINCREMENT,"
					"row_idx	INTEGER NOT NULL,"
					"column_idx INTEGER NOT NULL,"
					"count INTEGER NOT NULL,"
					"item_type INTEGER NOT NULL,"
					"FOREIGN KEY (item_type) REFERENCES items(enum))";

	QSqlQuery SqlQuery = QSqlQuery( __data_base );
	SqlQuery.exec( query.arg(name) );
}

void IG_DataBaseManager::loadAllItemsTo(IG_InventoryTable *pTable)
{
	QString query = "SELECT * FROM states_data ";
	QSqlQuery SqlQuery = QSqlQuery( __data_base );
	SqlQuery.exec( query );

	QHash<uint, item_params> all_items_params;

	while (SqlQuery.next())
	{
		int item_idx_idx	= SqlQuery.record().indexOf("item_idx");
		int state_idx		= SqlQuery.record().indexOf("state");
		int img_path_idx	= SqlQuery.record().indexOf("img_path");
		int snd_path_idx	= SqlQuery.record().indexOf("snd_path");

		item_params params;

		params.__idx = SqlQuery.record().value( item_idx_idx ).toUInt();
		params.__states.insert( SqlQuery.record().value( state_idx ).toInt(),
								SqlQuery.record().value( img_path_idx ).toString()
								);
		params.__snds.insert( SqlQuery.record().value( state_idx ).toInt(),
							  SqlQuery.record().value( snd_path_idx ).toString());

		if( all_items_params.contains( params.__idx ) )
			{
				all_items_params[params.__idx].__states.insert(SqlQuery.record().value( state_idx ).toInt(),
															   SqlQuery.record().value( img_path_idx ).toString());

				all_items_params[params.__idx].__snds.insert(SqlQuery.record().value( state_idx ).toInt(),
															   SqlQuery.record().value( snd_path_idx ).toString());
			}
		else
			all_items_params.insert( params.__idx, params );
	}

	query = "SELECT * FROM interact_type ";
	SqlQuery.exec( query );

	while (SqlQuery.next())
	{
		int item_idx_idx	= SqlQuery.record().indexOf("item_idx");
		int inct_type_idx	= SqlQuery.record().indexOf("inct_type");

		all_items_params[ SqlQuery.record().value( item_idx_idx ).toUInt() ].__interact_type = (InteractionType)SqlQuery.record().value( inct_type_idx ).toUInt();
	}


	for (auto params:all_items_params)
	{
		QList<int> all_states = params.__states.keys();
		auto itr_max_state = std::max_element(all_states.begin(), all_states.end());
		params.__max_state = *itr_max_state;
		params.__state = params.__max_state;
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

	QSqlQuery query = QSqlQuery( __data_base );

	query.exec(
				"CREATE TABLE IF NOT EXISTS items ( enum INTEGER PRIMARY KEY,"
				"name VARCHAR(20) NOT NULL UNIQUE, img_path VARCHAR(255), snd_path VARCHAR(255) )"
				);
}

void IG_DataBaseManager::disconnectSQLiteDB()
{
	__data_base.close();
}

