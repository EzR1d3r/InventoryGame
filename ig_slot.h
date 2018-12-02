#pragma once
#include <QStack>
#include <QTableWidgetItem>
#include <ig_item.h>

class IG_InventoryTable;
class IG_Net_Slot;

class IG_Slot : public QTableWidgetItem
{
public:
	IG_Slot(IG_InventoryTable * parent = nullptr);

	inline int getCount () const {return __items.count();}
	inline const QStack<IG_Item>& getItems() const {return __items;}
	inline IG_InventoryTable * getParent() const {return __parent;}
	inline IG_Item& getTop (){ return __items.top(); }
	inline QString getTopImg () const { return __items.count() ? __items.top().getImg() : "" ; }
	inline QString getTopSnd () const { return __items.count() ? __items.top().getSnd() : "" ; }
	inline bool isInfinite() const { return __infinite;}

	void tryStack (IG_Slot *pSlot );
	inline void setInfinite(bool infinite){ __infinite = infinite;}
	void addItem(const IG_Item& item);
	void hitLast();
	void externalChange(const IG_Net_Slot &slot_data);
	void clear();
private Q_SLOTS:
	void update();
private:
	bool __infinite;
	QStack<IG_Item> __items;
	IG_InventoryTable * __parent;
};

//класс для передачи информации о состоянии слота по сетке

class IG_Net_Slot
{
public:
	IG_Net_Slot(){}
	IG_Net_Slot(const IG_Slot * pSlot );

	qint32 row = 0;
	qint32 column = 0;
	qint32 count = 0;
	QString img_path = "";
	QString snd_path = "";
};

QDataStream &operator<<(QDataStream& out, const IG_Net_Slot& net_slot);
QDataStream &operator>>(QDataStream& out, IG_Net_Slot& net_slot);
