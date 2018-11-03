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
	QString snd = getTop().getSnd ();
	if (!top_state)
		__items.pop();
	else
		__items.top().setState( __items.top().getState() -1 );

	emit __parent->preChangeItem(this, snd);
	//по идее слот должен свой сигнал эмитить,
	//таблица ловить и эмитить свой, для краткости сразу через __parent
	update();
}

void IG_Slot::externalChange(const IG_Net_Slot& slot_data)
{
	__items.clear();
	setText( QString::number( slot_data.count ) );
	setTextAlignment (Qt::AlignRight | Qt::AlignBottom);
	setIcon( QIcon() );
	setIcon( QIcon( slot_data.img_path) );
}

void IG_Slot::update()
{
	setText( QString::number( getCount() ) );
	setTextAlignment (Qt::AlignRight | Qt::AlignBottom);
	setIcon( QIcon() );
	if ( getCount() ) setIcon( QIcon( __items.top().getImg()) );
	emit __parent->slotChanged( this );
}

IG_Net_Slot::IG_Net_Slot(const IG_Slot *pSlot):row(pSlot->row()),
						column(pSlot->column()), count(pSlot->getCount())
{
	qDebug() << "&&&" << pSlot->getTopImg() << pSlot->getTopSnd();
	img_path = pSlot->getTopImg();
	snd_path = pSlot->getTopSnd();

	qDebug() << "???" << img_path << snd_path;
}

QDataStream &operator<<(QDataStream &out, const IG_Net_Slot &slot)
{
	out << slot.row << slot.column << slot.count;
	out << slot.img_path.toStdString().c_str() << slot.snd_path.toStdString().c_str();
	return  out;
}

QDataStream &operator>>(QDataStream &in, IG_Net_Slot &slot)
{
	char * img_path;
	char * snd_path;
	in >> slot.row >> slot.column >> slot.count;
	in >> img_path >> snd_path;

	slot.img_path = QString(img_path);
	slot.snd_path = QString(snd_path);
	return  in;
}
