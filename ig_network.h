#pragma once
#include <QTcpServer>
#include <QTcpSocket>

class IG_Slot;
class IG_Net_Slot;

class IG_Server : public QTcpServer
{
	Q_OBJECT
public:
	IG_Server();
	void startServer();
	void stopServer();
	void incomingConnection(qintptr socketDescriptor);
public slots:
	void slotChanged(const IG_Slot *pSlot);
private:
	QByteArray prepareData(const IG_Slot * pSlot);
	void socketReady();
	void socketDisconnected();

	QTcpSocket * __socket = nullptr;
};


class IG_Client: public QObject
{
	Q_OBJECT
	public:
		IG_Client();
		void stopClient();
	signals:
		void newData(const IG_Net_Slot& slot_data);
	private:
		void unpackData(QByteArray data);
		void socketReady();
		void socketDisconnected();
		QTcpSocket * __socket = nullptr;
};
