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

void IG_Slot::tryStack(IG_Slot *pSlot)
{
	if (pSlot == this) return;
	if ( __items.isEmpty() )
	{
		__items.append( pSlot->getItems() );
		if (!pSlot->isInfinite()) pSlot->clear();
	}
	else if ( getTop().getIndex() == pSlot->getTop().getIndex() &&
			  (getTop().getInteractType() & InteractionType::Stack) )
	{
		__items.append( pSlot->getItems() );
		if (!pSlot->isInfinite()) pSlot->clear();
	}
	else
	{
		getTop().interactWith( pSlot->getTop() );
		pSlot->update();
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
	QString snd = getTop().getSnd ( top_state );
	if (!top_state)
		__items.pop();
	else
		__items.top().setState( __items.top().getState() -1 );

	emit __parent->preChangeItem(this, snd);
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
