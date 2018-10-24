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

void IG_DataBaseManager::connectInventory(IG_InventoryTable *pInventory)
{
	query_create_inventory_table( pInventory->objectName() );
	connect( pInventory, pInventory->slotChanged, this, query_change_slot);
}

void IG_DataBaseManager::diconnectInventory(IG_InventoryTable *pInventory)
{
	disconnect(pInventory, pInventory->slotChanged, 0, 0);
}

//void IG_DataBaseManager::addItemInDB(Fruit type, QString name, QString img_path, QString snd_path)
//{
//	QString query_add_item = "INSERT INTO items (enum, name, img_path, snd_path) "
//							 "VALUES (%1, '%2', '%3', '%4')";
//	QSqlQuery SqlQuery = QSqlQuery( __data_base );
//	SqlQuery.exec( query_add_item.arg(static_cast<int>(type)).arg(name).arg(img_path).arg(snd_path) );
//}

//QString IG_DataBaseManager::imgPathByType(Fruit type)
//{
//	QString query = "SELECT * FROM items WHERE enum = %1";
//	QSqlQuery SqlQuery = QSqlQuery( __data_base );
//	SqlQuery.exec( query.arg( static_cast<int>(type) ) );
//	if (SqlQuery.next())
//		return SqlQuery.record().field("img_path").value().toString();
//	else
//		return QString("");
//}

//QString IG_DataBaseManager::sndPathByType(Fruit type)
//{
//	QString query = "SELECT * FROM items WHERE enum = %1";
//	QSqlQuery SqlQuery = QSqlQuery( __data_base );
//	SqlQuery.exec( query.arg( static_cast<int>(type) ) );
//	if (SqlQuery.next())
//		return SqlQuery.record().field("snd_path").value().toString();
//	else
//		return QString("");
//}

void IG_DataBaseManager::query_change_slot(IG_Slot *pSlot)
{
	QString t_name = pSlot->getParent()->objectName();
	int count = pSlot->getCount();
	int type = static_cast<int> (pSlot->getItems().top().getIndex());
	int row = pSlot->row();
	int column = pSlot->column();

	QSqlQuery SqlQuery = QSqlQuery( __data_base );

	QString query_row = "SELECT * FROM %1 WHERE row_idx = %2 AND column_idx = %3";
	QString query_update = "UPDATE %1 SET count = %2, item_type = %3 "
					"WHERE row_idx = %4 AND column_idx = %5";
	QString query_insert = "INSERT INTO %1 (row_idx, column_idx, count, item_type) "
						   "VALUES(%2, %3, %4, %5)";

	SqlQuery.exec( query_row.arg(t_name).arg(row).arg(column) ); //проверяем если такая строка

	if (!SqlQuery.next())
		SqlQuery.exec( query_insert.arg(t_name).arg(row).arg(column).arg(count).arg(type) );
	else
		SqlQuery.exec( query_update.arg(t_name).arg(count).arg(type).arg(row).arg(column) );

}

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
//		qDebug() << SqlQuery.record().indexOf("item_idx");

		int item_idx_idx	= SqlQuery.record().indexOf("item_idx");
		int state_idx		= SqlQuery.record().indexOf("state");
		int img_path_idx	= SqlQuery.record().indexOf("img_path");
		int snd_path_idx	= SqlQuery.record().indexOf("snd_path");

		item_params params;

		params.__idx = SqlQuery.record().value( item_idx_idx ).toUInt();
		params.__states.insert( SqlQuery.record().value( state_idx ).toInt(),
								SqlQuery.record().value( img_path_idx ).toString()
								);
		params.__snd = SqlQuery.record().value( snd_path_idx ).toString();

		if( all_items_params.contains( params.__idx ) )
			{
				all_items_params[params.__idx].__states.insert(SqlQuery.record().value( state_idx ).toInt(),
															   SqlQuery.record().value( img_path_idx ).toString());

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

		all_items_params[ SqlQuery.record().value( item_idx_idx ).toInt() ].__interact_type = (InteractionType)SqlQuery.record().value( inct_type_idx ).toInt();
		qDebug() << "!!!!!!" << item_idx_idx << ": " << inct_type_idx;
	}

	for (auto params:all_items_params)
	{
		QList<int> all_states = params.__states.keys();
		auto itr_max_state = std::max_element(all_states.begin(), all_states.end());
		params.__state = *itr_max_state;
		IG_Item item( params );

		IG_Slot * pSlot = new IG_Slot ( pTable );
		pSlot->addItem( item );
		pSlot->setInfinite(true);
		pTable->setItem( params.__idx, 0, pSlot );
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

