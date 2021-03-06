#include <QDebug>
#include <QTableWidgetItem>
#include <QSound>
#include <QDir>
#include "ui_ig_mainwidget.h"
#include "ig_mainwidget.h"
#include <ig_MainMenu.h>
#include <ig_item.h>
#include <ig_slot.h>
#include <ig_network.h>
#include <ig_utils.h>

IG_MainWidget::IG_MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::IG_MainWidget)
{
	ui->setupUi(this);

	//логиченее делать fillBySlots() в конструкторе, но автоматическое создание
	//объекта в Qt сначала создает объект, а потом менят кол-во столбцов и колонок
	//как вариант перегрузить эти методы
	ui->twInventory->fillBySlots();

	//Main menu widget
	__main_menu = new IG_MainMenu();
	__net_manager = new IG_NetworkManager(ui->twInventory);
    connect(__main_menu, &IG_MainMenu::exit_game, this, &IG_MainWidget::exit_game);
    connect(__main_menu, &IG_MainMenu::new_game, this, &IG_MainWidget::new_game );
    connect(__main_menu, &IG_MainMenu::close_mm, this, &IG_MainWidget::setEnabled);
    connect(ui->twInventory, &IG_InventoryTable::slotItemChanged, this, &IG_MainWidget::playSnd);

    connect(__net_manager, &IG_NetworkManager::clientConnected, this, &IG_MainWidget::clientConnected);
    connect(__net_manager, &IG_NetworkManager::clientDisconnected, this, &IG_MainWidget::clientDisconnected);

	db.connectSQLiteDB( IG_Utils::absPathFromLocal( DB_PATH ) );
	db.loadAllItemsTo( ui->twStore );

	ui->leHostAddress->setText( "localhost:" + QString::number( DEF_PORT ));
}

IG_MainWidget::~IG_MainWidget()
{
	db.disconnectSQLiteDB();
	delete __net_manager;
	delete __main_menu;
	delete ui;
}

void IG_MainWidget::on_btnMainMenu_clicked()
{
	setEnabled(false);
	__main_menu->show();
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

void IG_MainWidget::playSnd(IG_Slot *pSlot, QString snd)
{
	Q_UNUSED( pSlot );
	QSound::play( IG_Utils::findSnd( snd ) );
}

void IG_MainWidget::becomeServer()
{
	ui->lbRole->setText( "SERVER" );
	ui->gbClientControls->setEnabled(0);
	ui->twStore->setEnabled(true);
	ui->twInventory->setDragDropMode( QAbstractItemView::DragDrop );
	__net_manager->becomeServer();
}

void IG_MainWidget::becomeClient()
{
	ui->lbRole->setText( "CLIENT" );
	ui->gbClientControls->setEnabled(1);
	ui->twStore->setEnabled(false);
	ui->twInventory->setDragDropMode( QAbstractItemView::NoDragDrop );
	__net_manager->becomeClient();
}

void IG_MainWidget::on_pbDisconnect_clicked()
{
	__net_manager->disconnectFromHost();
}

void IG_MainWidget::on_pbConnect_clicked()
{
	QStringList addr = ui->leHostAddress->text().split(":");
    __net_manager->connectToHost(addr[0], addr[1].toUShort());
}

void IG_MainWidget::exit_game(int i)
{
	exit(i);
}

void IG_MainWidget::clientConnected()
{
	ui->lbStatus->setText("Connected");
}

void IG_MainWidget::clientDisconnected()
{
	ui->lbStatus->setText("Disconnected");
}

void IG_MainWidget::show()
{
	QWidget::show();
	on_btnMainMenu_clicked();
}
