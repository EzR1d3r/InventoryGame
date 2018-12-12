#pragma once
#include <QtSql>
#include <ig_item.h>

class IG_InventoryTable;
class IG_Slot;

class IG_DataBaseManager: public QObject
{
	Q_OBJECT
public:
	IG_DataBaseManager();
	void connectSQLiteDB(QString name);
	void disconnectSQLiteDB();
	void loadAllItemsTo (IG_InventoryTable * pTable);
private:
	QSqlDatabase __data_base;
	static int __counter;
	int __ID;
};
