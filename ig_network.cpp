#include <QDebug>
#include <QDataStream>
#include <ig_slot.h>
#include "ig_network.h"

#define HOST_ADDRESS QHostAddress::LocalHost
#define PORT 1313

//Server

IG_Server::IG_Server()
{

}

void IG_Server::startServer()
{
	if ( listen( HOST_ADDRESS, PORT ) )
		qDebug() << "Server is ready";
}

void IG_Server::stopServer()
{
	if (__socket)
	{
		__socket->disconnectFromHost();
	}
}

void IG_Server::incomingConnection(qintptr socketDescriptor)
{
	if (__socket) return; // ограничение одним подключением
	__socket = new QTcpSocket(this);
	__socket->setSocketDescriptor( socketDescriptor );

	connect(__socket, __socket->readyRead, this, socketReady);
	connect(__socket, __socket->disconnected, this, socketDisconnected);
	qDebug() << "incomingConnection";
}

void IG_Server::slotChanged(const IG_Slot * pSlot)
{
	if (!__socket) return;
//	qDebug() << "========================";
//	qDebug() << "Server: recieved signal slot changed" << pSlot->row() << pSlot->column();
	__socket->write( prepareData( pSlot ) );
}

QByteArray IG_Server::prepareData(const IG_Slot *pSlot)
{
//	qDebug() << "Prepare data";
	QByteArray data;
	QDataStream out(&data, QIODevice::WriteOnly);
	IG_Net_Slot slot_data( pSlot );
	qDebug() << "!!!" << slot_data.img_path << slot_data.snd_path;
	out << slot_data;
	return data;
}

void IG_Server::socketReady()
{
	qDebug() << "Socket READY";
}

void IG_Server::socketDisconnected()
{
	disconnect(__socket,nullptr,nullptr,nullptr);
	delete __socket;
	__socket = nullptr;
	qDebug() << "Server: Socket disconnected";
}


//Client

IG_Client::IG_Client()
{
	__socket = new QTcpSocket(this);

	connect(__socket, __socket->readyRead, this, socketReady);
	connect(__socket, __socket->disconnected, this, socketDisconnected);

	__socket->connectToHost( HOST_ADDRESS, PORT );
}

void IG_Client::stopClient()
{
	if (__socket)
	{
		__socket->disconnectFromHost();
	}
}

void IG_Client::unpackData(QByteArray data)
{
//	qDebug() << "Unpack data";
	QDataStream in(&data, QIODevice::ReadOnly);

	IG_Net_Slot slot_data;
	in >> slot_data;

	qDebug() << slot_data.row << slot_data.column << slot_data.count;
	qDebug() << slot_data.img_path << slot_data.snd_path;

	emit newData(slot_data);
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
	disconnect(__socket,nullptr,nullptr,nullptr);
	delete __socket;
	__socket = nullptr;
	qDebug() << "Client: Socket disconnected";
}
