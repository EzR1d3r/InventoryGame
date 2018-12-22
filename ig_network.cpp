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

    connect(socket, &QTcpSocket::readyRead, this, &IG_Server::socketRead);
    connect(socket, &QTcpSocket::disconnected, this, &IG_Server::socketDisconnected);

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

    connect(__socket, &QTcpSocket::readyRead, this, &IG_Client::socketRead);
    connect(__socket, &QTcpSocket::disconnected, this, &IG_Client::socketDisconnected);
    connect( __socket,&QTcpSocket::connected, this, &IG_Client::socketConnected);

    connect( __socket,&QTcpSocket::connected, this, &IG_Client::connected);
    connect(__socket, &QTcpSocket::disconnected, this, &IG_Client::disconnected);
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

    connect(__client,  &IG_Client::disconnected, this, &IG_NetworkManager::clientDisconnected);
    connect( __client, &IG_Client::connected, this, &IG_NetworkManager::clientConnected);
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
    QObject::disconnect(__client, &IG_Client::newData, nullptr, nullptr);

	//сервер
    QObject::connect( __inventory, &IG_InventoryTable::slotChanged, this, &IG_NetworkManager::sendSlotToAllClients );
    QObject::connect( __server, &IG_Server::newConnection, this, &IG_NetworkManager::sendSlotsToNewClient);

	__server->startServer();
	__network_role = NetworkRole::Server;
}

void IG_NetworkManager::becomeClient()
{
	if (__network_role == NetworkRole::Client) return;
	//если ранее был сервером
    QObject::disconnect( __inventory, &IG_InventoryTable::slotChanged,
                      nullptr, nullptr);
    QObject::disconnect( __server, &IG_Server::newConnection,
					  nullptr, nullptr);
	__server->stopServer();

	//клиент
    QObject::connect(__client, &IG_Client::newData, this, &IG_NetworkManager::unpackData);
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
