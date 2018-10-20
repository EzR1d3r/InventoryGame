#include "ig_slot.h"
#include "ig_inventorytable.h"

IG_Slot::IG_Slot( IG_InventoryTable * parent ):
	__infinite(false), __parent (parent)
{
	update();
}

void IG_Slot::clear()
{
	__items.clear();
	update();
}

void IG_Slot::Interact(IG_Slot *pSlot)
{
	if (pSlot == this) return;
	if ( __items.isEmpty() )
	{
		__items.append( pSlot->getItems() );
		if (!pSlot->isInfinite()) pSlot->clear();
	}
	else if ( __items.top().getIndex() == pSlot->getItems().top().getIndex() )
	{
		__items.append( pSlot->getItems() );
		if (!pSlot->isInfinite()) pSlot->clear();
	}
	update();
}

void IG_Slot::addItem(const IG_Item &item)
{
	__items.push(item);
	update();
}

void IG_Slot::hitLast()
{
	if (__items.isEmpty() || __infinite) return;
	uint top_state = __items.top().getState();
	if (!top_state)
		__items.pop();
	else
		__items.top().setState( __items.top().getState() -1 );

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
