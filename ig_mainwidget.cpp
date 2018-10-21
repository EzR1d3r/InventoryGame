#include <QDebug>
#include <QTableWidgetItem>
#include <QSound>
#include "ui_ig_mainwidget.h"
#include "ig_MainWidget.h"
#include <ig_MainMenu.h>
#include <ig_item.h>
#include <ig_slot.h>
#include <ig_network.h>

#define PROJECT_PATH "f:/Projects/InventoryGame/%1"


IG_MainWidget::IG_MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::IG_MainWidget)
{
	ui->setupUi(this);

	//логиченее делать fillBySlots() в конструкторе, но автоматическое создание
	//объекта в Qt сначала создает объект, а потом менят кол-во столбцов и колонок
	//как вариант перегрузить эти методы
	ui->twInventory->fillBySlots();
	ui->twStore->fillBySlots();

	// Временно создаю итемы прямо тут, для отладки, потом будут грузиться из БД
//	QHash<int, QString> states;
//	for (int i = 0; i < 3; i++)
//		states.insert(i, QString(":/src/images/apple_%1.png").arg( QString::number( i ) ));

//	item_params params;
//	params.__idx = 1;
//	params.__name = "apple";
//	params.__state = 2;
//	params.__states = states;
//	params.__interact_type = InteractionType::Stack;

//	IG_Item item( params );
//	IG_Slot * pSlot = new IG_Slot( ui->twInventory );
//	pSlot->addItem( item );
//	pSlot->addItem( item );
//	pSlot->addItem( item );
//	IG_Slot * pSlot2 = new IG_Slot( ui->twInventory );
//	pSlot2->addItem( item );
//	pSlot2->addItem( item );
//	ui->twInventory->setItem(0, 0, pSlot);
//	ui->twInventory->setItem(1, 1, pSlot2);

	QHash<int, QString> states;
	for (int i = 0; i < 2; i++)
		states.insert(i, QString(":/src/images/vine_glass_%1.png").arg( QString::number( i ) ));

	item_params params;
	params.__idx = 2;
	params.__name = "glass_vine";
	params.__state = 1;
	params.__states = states;
	params.__interact_type = InteractionType::Volumes;

	IG_Item item( params );
	IG_Slot * pSlot = new IG_Slot( ui->twInventory );
	pSlot->addItem( item );
	IG_Slot * pSlot2 = new IG_Slot( ui->twInventory );
	pSlot2->addItem( item );
	ui->twInventory->setItem(0, 0, pSlot);
	ui->twInventory->setItem(1, 1, pSlot2);

	//Main menu widget
	mm = new IG_MainMenu();
	connect(mm, mm->exit_game, this, exit);
	connect(mm, mm->new_game, this, new_game);
	connect(mm, mm->close_mm, this, setEnabled);
//	connect(ui->twInventory, ui->twInventory->deleteOneItem, this, playSnd);

//	db.connectSQLiteDB( QString(PROJECT_PATH).arg("sqlite/inv_game.sqlite") );
}

IG_MainWidget::~IG_MainWidget()
{
//	db.disconnectSQLiteDB();
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
//	ui->twInventory->clear();

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

//	db.connectInventory( ui->twInventory );

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
