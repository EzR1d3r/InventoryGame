#include <QDebug>
#include <QDataStream>
#include <ig_slot.h>
#include "ig_server.h"

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

void IG_Server::slotChanged(IG_Slot * pSlot)
{
	if (!__socket) return;
//	qDebug() << "========================";
//	qDebug() << "Server: recieved signal slot changed" << pSlot->row() << pSlot->column();
	__socket->write( prepareData( pSlot ) );
}

QByteArray IG_Server::prepareData(IG_Slot *pSlot)
{
//	qDebug() << "Prepare data";
	QByteArray data;
	QDataStream out(&data, QIODevice::WriteOnly);
	out << qint32(pSlot->row());
	out << qint32(pSlot->column());
	out << qint32(pSlot->getCount());
	out << qint32(pSlot->getType());

	if (pSlot->getCount())
		out << pSlot->getItems().last().getImg().toStdString().c_str();
	else
		out << "";

	return data;
}

void IG_Server::socketReady()
{
	qDebug() << "Socket READY";
}

void IG_Server::socketDisconnected()
{
	disconnect(__socket,0,0,0);
	delete __socket;
	__socket = 0;
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

	int row, column, count, item_type;
	char * img_path;

	while (!in.atEnd())
	{
		in >> row;
		in >> column;
		in >> count;
		in >> item_type;
		in >> img_path;

		emit newData(row, column, count, item_type, img_path);
//		qDebug() << row << column << count << item_type << img_path << in.atEnd();
	}
}

void IG_Client::socketReady()
{
	qDebug() << "Client: Incoming data from server, socket: " << __socket;
	if (__socket && __socket->waitForConnected(500))
	{
		QByteArray data = __socket->readAll();
		unpackData( data );
	}
}

void IG_Client::socketDisconnected()
{
	disconnect(__socket,0,0,0);
	delete __socket;
	__socket = 0;
	qDebug() << "Client: Socket disconnected";
}
