#pragma once
#include <QStack>
#include <QTableWidgetItem>
#include <ig_item.h>

class IG_InventoryTable;

class IG_Slot : public QTableWidgetItem
{
public:
	IG_Slot(IG_InventoryTable * parent = 0);

	inline unsigned int getCount () const {return __items.count();}
	inline const QStack<IG_Item>& getItems() const {return __items;}
	inline IG_InventoryTable * getParent() const {return __parent;}
	inline IG_Item& getTop (){ return __items.top(); }
	inline bool isInfinite() const { return __infinite;}

	void tryStack (IG_Slot *pSlot );
	inline void setInfinite(bool infinite){ __infinite = infinite;}
	void addItem(const IG_Item& item);
	void hitLast();
	void externalChange(int count, const IG_Item &item);
	void clear();
private slots:
	void update();
private:
	bool __infinite;
	QStack<IG_Item> __items;
	IG_InventoryTable * __parent;
};
