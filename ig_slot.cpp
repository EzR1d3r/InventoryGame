#include "ig_inventorytable.h"
#include "ig_slot.h"


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

void IG_Slot::getFrom(IG_Slot *pSlot)
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

void IG_Slot::removeItem()
{
	if (!__items.count() || __infinite) return;
	__items.removeLast();
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
