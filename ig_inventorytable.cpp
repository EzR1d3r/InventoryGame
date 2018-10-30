#include <QDebug>
#include <QTableWidget>
#include <QMimeData>
#include <QDrag>
#include <QByteArray>
#include <QDropEvent>
#include <QDragEnterEvent>
#include "ig_inventorytable.h"
#include "ig_slot.h"

IG_InventoryTable::IG_InventoryTable(QWidget *parent) : QTableWidget(parent){}

void IG_InventoryTable::mousePressEvent(QMouseEvent *event)
{
	IG_Slot * pSource = dynamic_cast<IG_Slot*>(itemAt( event->pos() ));
	if (event->button() == Qt::MouseButton::RightButton)
	{
		pSource->hitLast();
		return;
	}

	QDrag * drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData;

	QString data = QString ("%1 %2").arg( QString::number(pSource->row() )).arg( QString::number(pSource->column()) );

	mimeData->setData("source_slot", data.toLocal8Bit());
	drag->setMimeData(mimeData);

	drag->exec( Qt::CopyAction | Qt::MoveAction );
	event->accept();
}

void IG_InventoryTable::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void IG_InventoryTable::dragMoveEvent(QDragMoveEvent *event)
{
	event->acceptProposedAction();
}

void IG_InventoryTable::dropEvent(QDropEvent *event)
{
	IG_Slot * pTargetSlot =  dynamic_cast<IG_Slot*> ( itemAt(event->pos()) );
	if (pTargetSlot)
	{
		IG_InventoryTable * pSourceInventory = qobject_cast<IG_InventoryTable*> ( event->source() );
		if (pSourceInventory)
		{
			const char * data = event->mimeData()->data( "source_slot" ).constData();
			QStringList r_c = QString( data ).split(" ");
			IG_Slot * pSourceSlot = pSourceInventory->getSlot( r_c[0].toInt(), r_c[1].toInt());
			pTargetSlot->tryStack( pSourceSlot );
		}
	}
}

void IG_InventoryTable::clear()
{
	for (int r = 0; r < rowCount(); r++)
		for (int c = 0; c < columnCount(); c++)
			getSlot(r, c)->clear();
}

void IG_InventoryTable::externalChange(int row, int column, int count, int item_type, const char *img_path)
{
//	getSlot(row, column)->externalChange(count, static_cast<Fruit>(item_type), QString(img_path));
	Q_UNUSED(row);
	Q_UNUSED(column);
	Q_UNUSED(count);
	Q_UNUSED(item_type);
	Q_UNUSED(img_path);
}

IG_Slot *IG_InventoryTable::getSlot(int row, int column)
{
	return dynamic_cast<IG_Slot*> ( item(row, column) );
}

void IG_InventoryTable::fillBySlots()
{
	for (int r = 0; r < rowCount(); r++)
		for (int c = 0; c < columnCount(); c++)
			setItem( r, c, new IG_Slot(this) );
}
