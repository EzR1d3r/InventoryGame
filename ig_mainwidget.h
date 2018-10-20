#pragma once
#include <QWidget>
#include <ig_databasemanager.h>
#include "ig_consts.h"

class IG_MainMenu;
class IG_Server;
class IG_Client;

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
	void new_game(NetworkRole role);
	void playSnd(IG_Slot * pSlot);

	void becomeServer();
	void becomeClient();
private:
//	IG_DataBaseManager db;
	Ui::IG_MainWidget *ui;
	IG_MainMenu * mm;
	IG_Server * __server = 0;
	IG_Client * __client = 0;
	NetworkRole __network_role = NetworkRole::None;
};
