#pragma once
#include <QTableWidget>

class IG_Slot;

class IG_InventoryTable : public QTableWidget
{
	Q_OBJECT
public:
	explicit IG_InventoryTable(QWidget *parent);

	IG_Slot * getSlot(int row, int column);
	void fillBySlots(); //чтобы не было нулевых указателей при itemAt(QPoint)

	void mousePressEvent(QMouseEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent( QDropEvent * event );
	void clear();
signals:
	void slotChanged(IG_Slot * pSlot);
	void deleteOneItem(IG_Slot * pSlot);
};
