#pragma once
#include <QTcpServer>
#include <QTcpSocket>

#define DEF_PORT 1313

class IG_Slot;
class IG_Net_Slot;

class IG_Server : public QTcpServer
{
	Q_OBJECT
public:
	IG_Server();
	~IG_Server();
	void startServer();
	void stopServer();
	void incomingConnection(qintptr socketDescriptor);
public slots:
	void slotChanged(const IG_Slot *pSlot);
private:
	QByteArray prepareData(const IG_Slot * pSlot);
	void socketReady();
	void socketDisconnected();

	QSet <QTcpSocket *> __sockets;
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
		void newData(const IG_Net_Slot& slot_data);
	private:
		void unpackData(QByteArray data);
		void socketReady();
		void socketDisconnected();
		QTcpSocket * __socket = nullptr;
};
