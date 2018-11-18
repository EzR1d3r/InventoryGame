#include <QDebug>
#include <QTableWidgetItem>
#include <QSound>
#include <QDir>
#include "ui_ig_mainwidget.h"
#include "ig_MainWidget.h"
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
	connect(__main_menu, __main_menu->exit_game, this, exit);
	connect(__main_menu, __main_menu->new_game, this, new_game);
	connect(__main_menu, __main_menu->close_mm, this, setEnabled);
	connect(ui->twInventory, ui->twInventory->slotItemChanged, this, playSnd);

//	QString dbPath = QDir(QCoreApplication::applicationDirPath() + "/../..").absolutePath() + "/sqlite/inv_game.sqlite";
	db.connectSQLiteDB( IG_Utils::absPath( DB_PATH ) );
	db.loadAllItemsTo( ui->twStore );
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
	QSound::play( snd );
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

void IG_MainWidget::show()
{
	QWidget::show();
	on_btnMainMenu_clicked();
}
