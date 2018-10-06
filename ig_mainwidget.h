#pragma once
#include <QDebug>
#include <QWidget>
#include <QTableWidgetItem>
#include <ig_item.h>
#include <ig_slot.h>
#include <ig_databasemanager.h>

namespace Ui
{
	class IG_MainWidget;
}

class IG_MainWidget : public QWidget
{
	Q_OBJECT

public:
	explicit IG_MainWidget(QWidget *parent = 0);
	~IG_MainWidget();

private:
	IG_DataBaseManager db;
	Ui::IG_MainWidget *ui;
};
