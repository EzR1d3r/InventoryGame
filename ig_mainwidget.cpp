#include <QDebug>
#include "ig_MainWidget.h"
#include "ui_ig_mainwidget.h"
#include <QTableWidgetItem>
#include <ig_item.h>
#include <ig_slot.h>

IG_MainWidget::IG_MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::IG_MainWidget)
{
	ui->setupUi(this);
	mm = new IG_MainMenu();

	connect(mm, mm->exit_game, this, exit);
	connect(mm, mm->new_game, this, new_game);

	ui->twInventory->fillBySlots();
	ui->twStore->fillBySlots();

	Item item(Fruit::Apple, ":/src/images/apple.png", "");
	ui->twStore->getSlot(0, 0)->addItem( item );
	ui->twStore->getSlot(0, 0)->setInfinite(true);

	QString db_path = "f:/Projects/InventoryGame/sqlite/inv_game.sqlite";
	db.connectSQLiteDB( db_path );
	db.connectInventory( ui->twInventory );
}

IG_MainWidget::~IG_MainWidget()
{
	db.disconnectSQLiteDB();
	delete mm;
	delete ui;
}

void IG_MainWidget::on_btnMainMenu_clicked()
{
	setEnabled(false);
	mm->show();
}

void IG_MainWidget::new_game()
{
	setEnabled(true);
	ui->twInventory->clear();
	QWidget::show();
}

void IG_MainWidget::show()
{
	QWidget::show();
	on_btnMainMenu_clicked();
}
