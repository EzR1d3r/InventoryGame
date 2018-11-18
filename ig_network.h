#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QQueue>
#include "ig_consts.h"

#define DEF_PORT 1313

class IG_Slot;
class IG_Net_Slot;
class IG_InventoryTable;

class IG_Server : public QTcpServer
{
	Q_OBJECT
public:
	void startServer();
	void stopServer();
	void incomingConnection(qintptr socketDescriptor);
public slots:
	void sendDataToAll(QByteArray data);
	void sendData(QByteArray data, QTcpSocket *socket);
	void sendAndDeq(QByteArray data, bool dequeue = true);
private:
	void socketRead();
	void socketDisconnected();

	QSet <QTcpSocket *> __sockets;
	QQueue <QTcpSocket *> __queue_sockets;
	//При новой соединении QTcpServer испускает сигнал, но никаких данных о сокете
	//не передает, поэтому новые сокеты набиваем в очередь и при поступлении команды
	//обрабатываем их. Возможно стоит сделать свой сигнал вроде newConnection (qintptr socketDescriptor)
	//
};


class IG_Client: public QObject
{
	Q_OBJECT
	public:
		IG_Client();
		~IG_Client(){}
		void connectToHost(const QString &hostName, quint16 port = DEF_PORT);
		void disconnectFromHost();
	signals:
		void newData(QByteArray data);
	private:
		void socketRead();
		void socketDisconnected();
		QTcpSocket * __socket = nullptr;
};

class IG_NetworkManager: public QObject
{
	Q_OBJECT
public:
	IG_NetworkManager(IG_InventoryTable * inventory);
	~IG_NetworkManager();

	void becomeServer();
	void becomeClient();

	void connectToHost(const QString &hostName, quint16 port = DEF_PORT);
	void disconnectFromHost();
public slots:
	void sendSlotToAllClients(IG_Slot * pSlot);
private slots:
	void sendSlotsToNewClient();
private:
	QByteArray prepareData(const IG_Slot * pSlot);
	QByteArray& prepareData(const IG_Slot * pSlot, QByteArray &data);
	void unpackData(QByteArray data);

	IG_Server * __server;
	IG_Client * __client;
	NetworkRole __network_role = NetworkRole::None;
	IG_InventoryTable * __inventory;
};
