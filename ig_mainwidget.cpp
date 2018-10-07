#include <QDebug>
#include <QTableWidgetItem>
#include <QSound>
#include "ui_ig_mainwidget.h"
#include "ig_MainWidget.h"
#include <ig_MainMenu.h>
#include <ig_item.h>
#include <ig_slot.h>
#include <ig_server.h>

#define PROJECT_PATH "f:/Projects/InventoryGame/%1"


IG_MainWidget::IG_MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::IG_MainWidget)
{
	ui->setupUi(this);

	//логиченее делать fillBySlots() в конструкторе, но автоматическое создание
	//объекта в Qt сначала создает объект, а потом менят кол-во столбцов и колонок
	//как вариант перегрузить эти методы
	ui->twInventory->fillBySlots();
	ui->twStore->fillBySlots();

	//Main menu widget
	mm = new IG_MainMenu();
	connect(mm, mm->exit_game, this, exit);
	connect(mm, mm->new_game, this, new_game);
	connect(mm, mm->close_mm, this, setEnabled);
	connect(ui->twInventory, ui->twInventory->deleteOneItem, this, playSnd);

	db.connectSQLiteDB( QString(PROJECT_PATH).arg("sqlite/inv_game.sqlite") );

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
	delete __server;
	delete __client;
	delete mm;
	delete ui;
}

void IG_MainWidget::on_btnMainMenu_clicked()
{
	setEnabled(false);
	mm->show();
}

void IG_MainWidget::new_game(NetworkRole role)
{

	if (role == NetworkRole::Server)
		becomeServer();
	else if (role == NetworkRole::Client)
		becomeClient();

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

void IG_MainWidget::becomeServer()
{
	if (__network_role == NetworkRole::Server) return;

//	if (__network_role == NetworkRole::Client) // если до этого был сервером
//	{
//		disconnect(__client, __client->newData, 0, 0);
//		__client->stopClient();
//		delete __client;
//		__client = 0;

//		ui->twStore->setEnabled(true);
//		ui->twInventory->setDragDropMode( QAbstractItemView::DragDrop );
//	}

	db.connectInventory( ui->twInventory );

	__server = new IG_Server();
	connect( ui->twInventory, ui->twInventory->slotChanged, __server, __server->slotChanged);
	__server->startServer();

	ui->lbRole->setText( "SERVER" );
	__network_role = NetworkRole::Server;
}

void IG_MainWidget::becomeClient()
{
	if (__network_role == NetworkRole::Client) return;

//	if (__network_role == NetworkRole::Server) // если до этого был сервером
//	{
//		__server->stopServer();
//		delete __server;
//		__server = 0;

//		disconnect(ui->twInventory, ui->twInventory->slotChanged,0 ,0);
//		db.diconnectInventory( ui->twInventory );
//	}

	__client = new IG_Client();
	connect(__client, __client->newData, ui->twInventory, ui->twInventory->externalChange);
	ui->twStore->setEnabled(false);
	ui->twInventory->setDragDropMode( QAbstractItemView::NoDragDrop );
	ui->lbRole->setText( "CLIENT" );
	__network_role = NetworkRole::Client;
}

void IG_MainWidget::show()
{
	QWidget::show();
	on_btnMainMenu_clicked();
}
