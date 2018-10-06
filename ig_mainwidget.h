#pragma once
#include <QWidget>
#include <ig_databasemanager.h>
#include <ig_MainMenu.h>

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

	void show();
private slots:
	void on_btnMainMenu_clicked();
	void new_game();
private:
	IG_DataBaseManager db;
	Ui::IG_MainWidget *ui;
	IG_MainMenu * mm;
};
