#pragma once
#include <QTableWidget>
#include <QDropEvent>
#include <QDragEnterEvent>
#include "ig_item.h"
#include "ig_slot.h"

class IG_InventoryTable : public QTableWidget
{
	Q_OBJECT
public:
	explicit IG_InventoryTable(QWidget *parent);

	IG_Slot * getSlot(int row, int column);
	void fillBySlots();
	void mousePressEvent(QMouseEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent( QDropEvent * event );

	inline void setName(QString name){ __name = name; }
	QString getName() const {return __name;}
signals:
	void slotChanged(IG_Slot * pSlot);
private:
	QString __name = "";
};
