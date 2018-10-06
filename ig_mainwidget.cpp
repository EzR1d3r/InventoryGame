#include "ig_MainWidget.h"
#include "ui_ig_mainwidget.h"

IG_MainWidget::IG_MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::IG_MainWidget)
{
	ui->setupUi(this);

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
	delete ui;
}
