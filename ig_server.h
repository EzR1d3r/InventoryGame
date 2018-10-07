#pragma once
#include <QTcpServer>
#include <QTcpSocket>

class IG_Slot;

class IG_Server : public QTcpServer
{
	Q_OBJECT
public:
	IG_Server();
	void startServer();
	void stopServer();
	void incomingConnection(qintptr socketDescriptor);
public slots:
	void slotChanged(IG_Slot * pSlot);
private:
	QByteArray prepareData(IG_Slot * pSlot);
	void socketReady();
	void socketDisconnected();

	QTcpSocket * __socket = 0;
};


class IG_Client: public QObject
{
	Q_OBJECT
	public:
		IG_Client();
		void stopClient();
	signals:
		void newData(int row, int column, int count, int item_type, const char * img_path);
	private:
		void unpackData(QByteArray data);
		void socketReady();
		void socketDisconnected();
		QTcpSocket * __socket = 0;
};
