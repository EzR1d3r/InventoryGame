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
	void connectInventory( IG_InventoryTable * pInventory );
	void addItemInDB(Fruit type, QString name, QString img_path, QString snd_path);

	QString imgPathByType (Fruit type);
	QString sndPathByType (Fruit type);
private slots:
	void query_change_slot(IG_Slot * pSlot);
	void query_create_inventory_table (QString name);
private:
	QSqlDatabase __data_base;
};
