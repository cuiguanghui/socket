#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H
#include <QTcpServer>
#include <QTcpSocket>

#define FMSOCKFLAG 0x4d46

class FMVipDispatcher;

typedef struct {
    int flag;
    int ver;
    int len;
}FMSOCKEHEADER;

class socketServer:public QTcpServer
{
public:
    socketServer();

    void Write(const QByteArray &data);
    int socketRecvData(char * recvBuf, int bufSize, QTcpSocket *sockClient);
private slots:
        void onNewConnection();
        void onDisconnected();
        void onReadyRead();
private:
    QTcpSocket *socket;
};

#endif // SOCKETSERVER_H
