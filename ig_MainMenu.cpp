#include "ig_MainMenu.h"
#include "ui_ig_mainmenu.h"
#include "ig_MainWidget.h"

IG_MainMenu::IG_MainMenu(QWidget *parent):
	QWidget(parent), ui(new Ui::ig_MainMenu)
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
		emit new_game(NetworkRole::Server);
	else
		emit new_game(NetworkRole::Client);
	ui->rbClient->setEnabled(false);
	ui->rbServer->setEnabled(false);
	close();
}

void IG_MainMenu::on_btnExit_clicked()
{
	emit exit_game(0);
	close();
}

void IG_MainMenu::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event);
	emit close_mm(true);
}
