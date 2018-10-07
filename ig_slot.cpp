#include "ig_slot.h"
#include "ig_inventorytable.h"

IG_Slot::IG_Slot( IG_InventoryTable * parent ):
	__type(Fruit::None), __infinite(false), __parent (parent)
{
	update();
}

void IG_Slot::clear()
{
	__items.clear();
	__type = Fruit::None;
	update();
}

void IG_Slot::takeFrom(IG_Slot *pSlot)
{
	if (pSlot == this) return;
	if ( __type == Fruit::None)
	{
		__type = pSlot->getType();
		__items.append( pSlot->getItems() );
		if (!pSlot->isInfinite()) pSlot->clear();
	}
	else if ( __type == pSlot->getType() )
	{
		__items.append( pSlot->getItems() );
		if (!pSlot->isInfinite()) pSlot->clear();
	}
	update();
}

void IG_Slot::addItem(const Item &item)
{
	if (__type == Fruit::None)
	{
		__type = item.getType();
		__items.append(item);
	}
	else if ( __type == item.getType() )
	{
		__items.append(item);
	}
	update();
}

void IG_Slot::addItem(Fruit type, QString img_path, QString snd_path)
{
	Item item(type, img_path, snd_path);
	addItem(item);
}

void IG_Slot::removeLast()
{
	if (!__items.count() || __infinite) return;
	__items.removeLast();
	emit __parent->deleteOneItem(this);
	//по идее слот должен свой сигнал эмитить,
	//таблица ловить и эмитить свой, для краткости сразу через __parent
	update();
}

void IG_Slot::externalChange(int count, Fruit type, QString img_path)
{
	__items.clear();
	__type = type;
	__items.fill ( Item(type, img_path, ""), count );
	update();
}

void IG_Slot::update()
{
	setText( QString::number( getCount() ) );
	setTextAlignment (Qt::AlignRight | Qt::AlignBottom);
	setIcon( QIcon() );
	if ( getCount() ) setIcon( QIcon( __items.last().getImg() ) );
	emit __parent->slotChanged( this );
}
