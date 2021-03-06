#include "ig_slot.h"
#include "ig_inventorytable.h"
#include "ig_utils.h"

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

void IG_Slot::collisionWith(IG_Slot *pSlot)
{
	if (pSlot == this) return;


	if ( __items.isEmpty() )
	{
		__items.append( pSlot->getItems() );
		if (!pSlot->isInfinite()) pSlot->clear();
	}
	else if ( getTop().checkStackable(pSlot->getTop()) )
	{
		__items.append( pSlot->getItems() );
		if (!pSlot->isInfinite()) pSlot->clear();
	}
	else
	{
		getTop().interactWith( pSlot->getTop() );
		pSlot->update();
	}

	QString snd = getTop().getSnd ();
	Q_EMIT __parent->slotItemChanged(this, snd);

	if (getTop().getState() == -1)
	{
		__items.pop();
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

	__items.top().setState( __items.top().getState() -1 );
	QString snd = getTop().getSnd ();
	if ( __items.top().getState() == -1 )
	{
		Q_EMIT __parent->slotChanged( this );
		__items.pop();
	}

	Q_EMIT __parent->slotItemChanged(this, snd);
	//по идее слот должен свой сигнал эмитить,
	//таблица ловить и эмитить свой, для краткости сразу через __parent
	update();
}

void IG_Slot::externalChange(const IG_Net_Slot& slot_data)
{
	__items.clear();
	setText( QString::number( slot_data.count ) );
	setTextAlignment (Qt::AlignRight | Qt::AlignBottom);
	QString img_path = IG_Utils::findImg( slot_data.img_path );
	setIcon( QIcon( img_path) );
	Q_EMIT __parent->slotItemChanged(this, slot_data.snd_path);
}

void IG_Slot::update()
{
	setText( QString::number( getCount() ) );
	setTextAlignment (Qt::AlignRight | Qt::AlignBottom);
	setIcon( QIcon() );
	if ( getCount() )
	{
		QString img_path = IG_Utils::findImg( __items.top().getImg() );
		setIcon( QIcon( img_path) );
	}
	Q_EMIT __parent->slotChanged( this );
}

//Net Slot

IG_Net_Slot::IG_Net_Slot(const IG_Slot *pSlot):row(pSlot->row()),
						column(pSlot->column()), count(pSlot->getCount())
{
	img_path = pSlot->getTopImg();
	snd_path = pSlot->getTopSnd();
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
