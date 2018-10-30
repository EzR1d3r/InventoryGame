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
public slots:
	void externalChange(int row, int column, int count, int item_type, const char * img_path);
signals:
	void slotChanged(IG_Slot * pSlot);
	void preChangeItem(IG_Slot * pSlot, QString snd); //можно все свойства итема эмитить или вообще копию
};
