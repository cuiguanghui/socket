#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QByteArray arr;
    arr="1122334455667788";
    SendToServer(arr);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SendToServer(const QByteArray &data)
{
    QTcpSocket client;

    qDebug() << "SendToMonitor to pos: " << data;
    int jsonLength=data.length();
    char* m_pFmPackage = new char[jsonLength + sizeof(FMSOCKEHEADER)];
    FMSOCKEHEADER header = { 0, 0, 0 };
    header.flag = 0x4d46;
    header.len = jsonLength;
    header.ver = 0x1;

    memcpy(m_pFmPackage, &header, sizeof(FMSOCKEHEADER));
    memcpy(m_pFmPackage+sizeof(FMSOCKEHEADER), data, jsonLength);


    int toSendLength = jsonLength + sizeof(FMSOCKEHEADER);
    int curSendLength = 0;

    client.connectToHost("127.0.0.1", 30001);
    if (client.waitForConnected()){
        while(curSendLength < toSendLength)
        {
            int rlt= client.write(m_pFmPackage+curSendLength,toSendLength - curSendLength);
            client.waitForBytesWritten();
            if(rlt==-1){
                qDebug()  << "发送数据失败：" << data;
                break;
            }
            curSendLength += rlt;
        }
    }
    else{
        qDebug()  << "socket 连接失败！";
    }
    client.close();
    delete[] m_pFmPackage;
}
