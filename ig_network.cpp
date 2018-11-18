#include <QDebug>
#include <QDataStream>
#include <ig_slot.h>
#include <ig_inventorytable.h>
#include "ig_network.h"

#define HOST_ADDRESS QHostAddress::AnyIPv4

// ==================================== Server =======================================
void IG_Server::startServer()
{
	if ( listen( HOST_ADDRESS, DEF_PORT ) )
		qDebug() << "Server is ready";
}

void IG_Server::stopServer()
{
	for ( auto socket: __sockets )
	{
		socket->disconnectFromHost();
		socket->deleteLater();
	}
	__sockets.clear();
}

void IG_Server::incomingConnection(qintptr socketDescriptor)
{
	QTcpSocket * socket = new QTcpSocket(this);
	socket->setSocketDescriptor( socketDescriptor );
	__sockets << socket;

	connect(socket, socket->readyRead, this, socketRead);
	connect(socket, socket->disconnected, this, socketDisconnected);

	__queue_sockets.enqueue(socket);
}

void IG_Server::sendDataToAll(QByteArray data)
{
	for ( auto socket: __sockets )
		socket->write( data );
}

void IG_Server::sendData(QByteArray data, QTcpSocket * socket)
{
	socket->write(data);
}

void IG_Server::sendAndDeq(QByteArray data, bool dequeue)
{
	if ( __queue_sockets.isEmpty() ) return;

	if (dequeue)
		__queue_sockets.dequeue()->write( data );
	else
		__queue_sockets.head()->write(data);
}


void IG_Server::socketRead()
{
	qDebug() << "Socket READY";
}

void IG_Server::socketDisconnected()
{
	qDebug() << "Server: Socket disconnected ";
	__sockets.remove( dynamic_cast<QTcpSocket*>( sender() ) );
	sender()->deleteLater();
}


//======================================= Client ======================================

IG_Client::IG_Client()
{
	__socket = new QTcpSocket(this);

	connect(__socket, __socket->readyRead, this, socketRead);
	connect(__socket, __socket->disconnected, this, socketDisconnected);
}

void IG_Client::connectToHost(const QString &hostName, quint16 port)
{
	__socket->connectToHost( hostName, port );
}

void IG_Client::disconnectFromHost()
{
	if (__socket) __socket->disconnectFromHost();
}

void IG_Client::socketRead()
{
//	qDebug() << "Client: Incoming data from server, socket: " << __socket;
	QByteArray data;
	if (__socket && __socket->waitForConnected(500))
		data = __socket->readAll();

	emit newData(data);
}

void IG_Client::socketDisconnected()
{
	qDebug() << "Client: Socket disconnected";
}


//================================ NetworkManager ===================================

IG_NetworkManager::IG_NetworkManager(IG_InventoryTable *inventory):__inventory(inventory)
{
	__server = new IG_Server();
	__client = new IG_Client();
}

IG_NetworkManager::~IG_NetworkManager()
{
	delete  __server;
	delete  __client;
}

QByteArray IG_NetworkManager::prepareData(const IG_Slot *pSlot)
{
	//	qDebug() << "Prepare data";
	QByteArray data;
	QDataStream out(&data, QIODevice::WriteOnly);
	IG_Net_Slot slot_data( pSlot );
	out << slot_data;
	return data;
}

QByteArray& IG_NetworkManager::prepareData(const IG_Slot *pSlot, QByteArray &data)
{
	data.append( prepareData(pSlot) );
	return data;
}

void IG_NetworkManager::unpackData(QByteArray data)
{
//	qDebug() << "Unpack data";
	QDataStream in(&data, QIODevice::ReadOnly);

	while ( !in.atEnd() )
	{
		IG_Net_Slot slot_data;
		in >> slot_data;
		__inventory->externalChange(slot_data);
	}
}

void IG_NetworkManager::becomeServer()
{
	if (__network_role == NetworkRole::Server) return;

	QObject::connect( __inventory, __inventory->slotChanged,
					  this, this->sendSlotToAllClients);
	QObject::connect( __server, __server->newConnection,
					  this, this->sendSlotsToNewClient);

	__server->startServer();
	__network_role = NetworkRole::Server;
}

void IG_NetworkManager::becomeClient()
{
	if (__network_role == NetworkRole::Client) return;

	QObject::connect(__client, __client->newData, this, this->unpackData);
	__network_role = NetworkRole::Client;
}

void IG_NetworkManager::connectToHost(const QString &hostName, quint16 port)
{
	__client->connectToHost( hostName, port );
}

void IG_NetworkManager::disconnectFromHost()
{
	__client->disconnectFromHost();
}

void IG_NetworkManager::sendSlotToAllClients(IG_Slot *pSlot)
{
	QByteArray data = prepareData(pSlot);
	__server->sendDataToAll( data );
}

void IG_NetworkManager::sendSlotsToNewClient()
{
	QVector<IG_Slot*> Slots =  __inventory->getSlots();
	QByteArray data;
	for (IG_Slot*pSlot:Slots)
		prepareData(pSlot, data);
	__server->sendAndDeq(data);
}
