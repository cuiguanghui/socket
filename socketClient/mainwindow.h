#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

#define FMSOCKFLAG 0x4d46

class FMVipDispatcher;

typedef struct {
    int flag;
    int ver;
    int len;
}FMSOCKEHEADER;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SendToServer(const QByteArray &data);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
