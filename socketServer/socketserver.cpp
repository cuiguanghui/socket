#include "socketserver.h"
#define BUFFER_SIZE             1024 //缓冲区大小

socketServer::socketServer()
    :    QTcpServer(),
         socket(nullptr)
{
    if(!this->listen(QHostAddress::Any, 30001)) {
        qDebug() << "Listen error: " << this->errorString();
    }
    connect(this, &QTcpServer::newConnection, this, &socketServer::onNewConnection);
}

void socketServer::onNewConnection()
{
    socket = nextPendingConnection();

    connect(socket, &QTcpSocket::disconnected, this, &socketServer::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &socketServer::onReadyRead);
}

void socketServer::onDisconnected()
{
    qDebug() << "Socket disconnected.";
    socket->close();
}

void socketServer::onReadyRead()
{
    qDebug() << "onReadyRead.";
    char* recvBuf=new char[BUFFER_SIZE*10];
    socketRecvData(recvBuf, BUFFER_SIZE*10, socket);
    qDebug() << "onReadyRead data:"<<recvBuf;

    if(socket->isValid()){
        //Write(rspData);
    }

    socket->waitForDisconnected();

    socket->deleteLater();
    socket = nullptr;

}

void socketServer::Write(const QByteArray &data)
{

}

//socket 数据接收
//返回值   成功：接收长度
//         失败：  -1
int socketServer::socketRecvData(char * recvBuf, int bufSize, QTcpSocket *sockClient)
{
    int rcvedLength = 0;
    int rcvFlag = 0; //firstly, to rcv header
    int needLength = sizeof(FMSOCKEHEADER);
    int rlt = 0;

    FMSOCKEHEADER headx;
    char * pcRcvBufer = (char *)(&headx);

    while(0 < needLength)
    {
        //接收并打印客户端数据
        int length = sockClient->read(pcRcvBufer + rcvedLength,needLength);
        if (0 < length)
        {
            needLength -= length;
            rcvedLength += length;

            if (0 == needLength)
            {
                if (0 == rcvFlag)
                {//header rcved complete
                    rcvFlag = 1;//to rcv payload

                    int flag = headx.flag;
                    int len  = headx.len;
                    int ver  = headx.ver;

                    if(len > bufSize)
                    {
                        rlt = -1;
                        qDebug() << "socket recv 接收数据长度大于接收缓冲区："<< len;
                        break;
                    }
                    needLength = len;
                    pcRcvBufer = recvBuf;
                    rcvedLength = 0;
                    continue;
                }
                else
                {
                    rlt = rcvedLength;
                    break;
                }
            }
        }
        else if(length == 0)
        {//socket error
            rlt = -1;
            qDebug() << "socket recv 返回值:" << length;
        }
    }
    recvBuf[rlt]='\0';
    return rlt;
}
