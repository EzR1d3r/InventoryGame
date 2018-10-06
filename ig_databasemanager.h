#pragma once
#include <QtSql>

class IG_InventoryTable;
class IG_Slot;

class IG_DataBaseManager: public QObject
{
	Q_OBJECT
public:
	IG_DataBaseManager();
	void connectSQLiteDB(QString name);
	void disconnectSQLiteDB();
	void connectInventory( IG_InventoryTable * pInventory );
private slots:
	void query_change_slot(IG_Slot * pSlot);
	void query_create_inventory_table (QString name);
private:
	QSqlDatabase m_DataBase;
};
