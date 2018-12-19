#include "ig_MainMenu.h"
#include "ui_ig_mainmenu.h"
#include "ig_mainwidget.h"

IG_MainMenu::IG_MainMenu(QWidget *parent): QWidget(parent), ui(new Ui::ig_MainMenu)
{
	ui->setupUi(this);
}

IG_MainMenu::~IG_MainMenu()
{
	delete ui;
}

void IG_MainMenu::on_btnNewGame_clicked()
{
	if (ui->rbServer->isChecked())
		Q_EMIT new_game(NetworkRole::Server);
	else
		Q_EMIT new_game(NetworkRole::Client);
//	ui->rbClient->setEnabled(false);
//	ui->rbServer->setEnabled(false);
	close();
}

void IG_MainMenu::on_btnExit_clicked()
{
	Q_EMIT exit_game(0);
	close();
}

void IG_MainMenu::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event);
	Q_EMIT close_mm(true);
}
