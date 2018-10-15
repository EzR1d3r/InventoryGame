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
	inline uint getType() const {return __type;}
	inline QStack<IG_Item> getItems() const {return __items;}
	inline IG_InventoryTable * getParent() const {return __parent;}
	inline bool isInfinite() const { return __infinite;}

	void takeFrom (IG_Slot *pSlot );
	inline void setInfinite(bool infinite){ __infinite = infinite;}
	void addItem(const IG_Item& item);
	void addItem(Fruit type, QString img_path, QString snd_path);
	void removeLast();
	void externalChange(int count, const IG_Item &item);
	void clear();
private slots:
	void update();
private:
	uint __type;
	bool __infinite;
	QStack<IG_Item> __items;
	IG_InventoryTable * __parent;
};
