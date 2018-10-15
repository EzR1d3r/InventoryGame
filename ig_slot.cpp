#include "ig_slot.h"
#include "ig_inventorytable.h"

IG_Slot::IG_Slot( IG_InventoryTable * parent ):
	__type(0), __infinite(false), __parent (parent)
{
	update();
}

void IG_Slot::clear()
{
	__items.clear();
	__type = 0;
	update();
}

void IG_Slot::takeFrom(IG_Slot *pSlot)
{
	if (pSlot == this) return;
	if ( __type == 0)
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

void IG_Slot::addItem(const IG_Item &item)
{
	if (__type == 0)
	{
		__type = item.getIndex();
		__items.push(item);
	}
	else if ( __type == item.getIndex() )
	{
		__items.push(item);
	}
	update();
}

void IG_Slot::removeLast()
{
	if (__items.isEmpty() || __infinite) return;
	__items.pop();
	if ( __items.isEmpty() ) __type = 0;
	emit __parent->deleteOneItem(this);
	//по идее слот должен свой сигнал эмитить,
	//таблица ловить и эмитить свой, для краткости сразу через __parent
	update();
}

void IG_Slot::externalChange(int count, const IG_Item& item)
{
	//заглушка
	update();
}

void IG_Slot::update()
{
	setText( QString::number( getCount() ) );
	setTextAlignment (Qt::AlignRight | Qt::AlignBottom);
	setIcon( QIcon() );
	if ( getCount() ) setIcon( QIcon( __items.top().getImg( __items.top().getState() )));
	emit __parent->slotChanged( this );
}
