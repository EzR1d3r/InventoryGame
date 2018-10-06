#include "ig_databasemanager.h"
#include "ig_inventorytable.h"


IG_DataBaseManager::IG_DataBaseManager(){}


void IG_DataBaseManager::connectInventory(IG_InventoryTable *pInventory)
{
	query_create_inventory_table( pInventory->objectName() );
	connect( pInventory, pInventory->slotChanged, this, query_change_slot);
}

void IG_DataBaseManager::query_change_slot(IG_Slot *pSlot)
{
	QString t_name = pSlot->getParent()->objectName();
	int count = pSlot->getCount();
	int type = static_cast<int> (pSlot->getType());
	int row = pSlot->row();
	int column = pSlot->column();

	QSqlQuery SqlQuery = QSqlQuery( m_DataBase );

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

	QSqlQuery SqlQuery = QSqlQuery( m_DataBase );
	SqlQuery.exec( query.arg(name) );
}

void IG_DataBaseManager::connectSQLiteDB(QString name)
{
	m_DataBase = QSqlDatabase::addDatabase("QSQLITE");
	m_DataBase.setDatabaseName(name);

	if (!m_DataBase.open())
	{
		qDebug() << "Error open database: " << name;
		return;
	}

	QSqlQuery query = QSqlQuery( m_DataBase );

	query.exec(
				"CREATE TABLE IF NOT EXISTS items ( enum INTEGER PRIMARY KEY AUTOINCREMENT,"
				"name VARCHAR(20) NOT NULL UNIQUE, img_path VARCHAR(255), img_snd VARCHAR(255) )"
				);

	query.exec( "INSERT INTO items (name, img_path) VALUES ('apple', ':/src/images/apple.png')" );
}

void IG_DataBaseManager::disconnectSQLiteDB()
{
	m_DataBase.close();
}

