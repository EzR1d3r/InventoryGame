#include <QDebug>
#include <QDataStream>
#include <ig_slot.h>
#include <ig_inventorytable.h>
#include "ig_network.h"

// ==================================== Server =======================================
void IG_Server::startServer()
{
	if ( listen( HOST_ADDRESS, DEF_PORT ) )
		qDebug() << "Server is ready";
}

void IG_Server::stopServer()
{
	//необходимо делать копию __sockets, тк сокеты при дисконнекте удаляются из __sockets
	QSet<QTcpSocket*> sockets = __sockets;
	for ( auto socket: sockets )
		socket->disconnectFromHost();
	__sockets.clear();
	__queue_sockets.clear();
	close();
}

void IG_Server::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << "incomingConnection: " << socketDescriptor;
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
	QTcpSocket* socket = dynamic_cast<QTcpSocket*>( sender() );
	__sockets.remove( socket );
	socket->deleteLater();
	qDebug() << "Server: Socket disconnected ";
}


//======================================= Client ======================================

IG_Client::IG_Client()
{
	__socket = new QTcpSocket(this);

	connect(__socket, __socket->readyRead, this, socketRead);
	connect(__socket, __socket->disconnected, this, socketDisconnected);
	connect( __socket, __socket->connected, this, socketConnected);

	connect( __socket, __socket->connected, this, connected);
	connect(__socket, __socket->disconnected, this, disconnected);
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

	Q_EMIT newData(data);
}

void IG_Client::socketDisconnected()
{
	qDebug() << "Client: Socket disconnected";
}

void IG_Client::socketConnected()
{
	qDebug() << "Client: Socket connected";
}


//================================ NetworkManager ===================================

IG_NetworkManager::IG_NetworkManager(IG_InventoryTable *inventory):__inventory(inventory)
{
	__server = new IG_Server();
	__client = new IG_Client();

	connect(__client, __client->disconnected, this, clientDisconnected);
	connect( __client, __client->connected, this, clientConnected);
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
	//если ранее был клиентом
	QObject::disconnect(__client, __client->newData, nullptr, nullptr);

	//сервер
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
	//если ранее был сервером
	QObject::disconnect( __inventory, __inventory->slotChanged,
					  nullptr, nullptr);
	QObject::disconnect( __server, __server->newConnection,
					  nullptr, nullptr);
	__server->stopServer();

	//клиент
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
