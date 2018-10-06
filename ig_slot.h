#pragma once
#include <QTableWidgetItem>
#include <ig_item.h>

class IG_InventoryTable;

class IG_Slot : public QTableWidgetItem
{
public:
	IG_Slot(IG_InventoryTable * parent = 0);
	inline unsigned int getCount () const {return __items.count();}
	inline Fruit getType() const {return __type;}
	inline QVector<Item> getItems(){return __items;}
	inline void clear();
	void getFrom (IG_Slot *pSlot );
	inline void setInfinite(bool infinite){ __infinite = infinite;}
	inline bool isInfinite(){ return __infinite;}
	void addItem(const Item& item);
	inline IG_InventoryTable * getParent(){return __parent;}
	void removeItem();
private slots:
	void update();
private:
	Fruit __type;
	bool __infinite;
	QVector<Item> __items;
	IG_InventoryTable * __parent;
};
