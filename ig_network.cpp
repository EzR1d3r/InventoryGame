#include <QDebug>
#include <QDataStream>
#include <ig_slot.h>
#include "ig_network.h"

#define HOST_ADDRESS QHostAddress::AnyIPv4

//Server

IG_Server::IG_Server()
{
}

IG_Server::~IG_Server()
{
}

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

	connect(socket, socket->readyRead, this, socketReady);
	connect(socket, socket->disconnected, this, socketDisconnected);
}

void IG_Server::sendSingleSlot(const IG_Slot * pSlot)
{
	for ( auto socket: __sockets )
	{
		socket->write( prepareData( pSlot ) );
	}
}

void IG_Server::sendSlots(const QVector<IG_Slot *> Slots)
{
	Q_UNUSED(Slots);
	qDebug() << "Server: sendSlots";
}

QByteArray IG_Server::prepareData(const IG_Slot *pSlot)
{
//	qDebug() << "Prepare data";
	QByteArray data;
	QDataStream out(&data, QIODevice::WriteOnly);
	IG_Net_Slot slot_data( pSlot );
	out << slot_data;
	return data;
}

void IG_Server::socketReady()
{
	qDebug() << "Socket READY";
}

void IG_Server::socketDisconnected()
{
	qDebug() << "Server: Socket disconnected ";
	__sockets.remove( dynamic_cast<QTcpSocket*>( sender() ) );
	sender()->deleteLater();
}


//Client

IG_Client::IG_Client()
{
	__socket = new QTcpSocket(this);

	connect(__socket, __socket->readyRead, this, socketReady);
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

void IG_Client::unpackData(QByteArray data)
{
//	qDebug() << "Unpack data";
	QDataStream in(&data, QIODevice::ReadOnly);

	while ( !in.atEnd() )
	{
		IG_Net_Slot slot_data;
		in >> slot_data;
		emit newData(slot_data);
	}
}

void IG_Client::socketReady()
{
//	qDebug() << "Client: Incoming data from server, socket: " << __socket;
	if (__socket && __socket->waitForConnected(500))
	{
		QByteArray data = __socket->readAll();
		unpackData( data );
	}
}

void IG_Client::socketDisconnected()
{
	qDebug() << "Client: Socket disconnected";
}
