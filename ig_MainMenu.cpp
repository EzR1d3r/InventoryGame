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
	emit new_game();
	close();
}

void IG_MainMenu::on_btnExit_clicked()
{
	emit exit_game(0);
	close();
}
