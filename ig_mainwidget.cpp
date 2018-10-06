#include <QDebug>
#include <QTableWidgetItem>
#include <QSound>
#include "ui_ig_mainwidget.h"
#include "ig_MainWidget.h"
#include <ig_MainMenu.h>
#include <ig_item.h>
#include <ig_slot.h>

#define PROJECT_PATH "f:/Projects/InventoryGame/%1"


IG_MainWidget::IG_MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::IG_MainWidget)
{
	ui->setupUi(this);
	mm = new IG_MainMenu();

	//логиченее делать fillBySlots() в конструкторе, но атоматическое создание
	//объекта в Qt сначала создает объект, а потом менят кол-во столбцов и колонок
	//как вариант перегрузить эти методы
	ui->twInventory->fillBySlots();
	ui->twStore->fillBySlots();

	connect(mm, mm->exit_game, this, exit);
	connect(mm, mm->new_game, this, new_game);
	connect(mm, mm->close_mm, this, setEnabled);
	connect(ui->twInventory, ui->twInventory->deleteOneItem, this, playSnd);

	db.connectSQLiteDB( QString(PROJECT_PATH).arg("sqlite/inv_game.sqlite") );
	db.connectInventory( ui->twInventory );

	db.addItemInDB( Fruit::Apple, "apple", ":/src/images/apple.png", ":/src/snds/apple.wav");
	db.addItemInDB( Fruit::Banana, "banana", ":/src/images/banana.png", ":/src/snds/banana.wav");
	db.addItemInDB( Fruit::Qiwi, "qiwi", ":/src/images/qiwi.png", "");

	ui->twStore->getSlot(0, 0)->addItem( Fruit::Apple, db.imgPathByType(Fruit::Apple), db.sndPathByType( Fruit::Apple ) );
	ui->twStore->getSlot(0, 0)->setInfinite(true);

	ui->twStore->getSlot(1, 0)->addItem( Fruit::Banana, db.imgPathByType(Fruit::Banana), db.sndPathByType( Fruit::Banana ) );
	ui->twStore->getSlot(1, 0)->setInfinite(true);

	ui->twStore->getSlot(2, 0)->addItem( Fruit::Qiwi, db.imgPathByType(Fruit::Qiwi), db.sndPathByType( Fruit::Qiwi) );
	ui->twStore->getSlot(2, 0)->setInfinite(true);
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

void IG_MainWidget::playSnd(IG_Slot *pSlot)
{
	//по тз храним звук файла в классе Item,
	//но когда "съедается" последний в слоте, звук брать неоткуда
	//либо сигналом передавать именно путь, либо экземпляр итема
	if (pSlot->getCount())
		QSound::play(pSlot->getItems().last().getSnd());
}

void IG_MainWidget::show()
{
	QWidget::show();
	on_btnMainMenu_clicked();
}
