#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpSocket *ReadWriteSocket;

public slots:
    void addOutText(QString);
    void addInText(QString);
    void startConnection(QString);

private slots:
    void callConnectDialog();
    void recvMessage();
    void SendMove(int,int,int,int,int,int);
    void sendstart();
    void sendreadgame(QString);
    void sendsavegame();
    void on_GiveUpBtn_clicked();
};

#endif // MAINWINDOW_H
