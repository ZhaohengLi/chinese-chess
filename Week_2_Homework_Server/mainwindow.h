#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString getIP(QString);

private:
    Ui::MainWindow *ui;
    QTcpServer  *listenSocket;
    QTcpSocket  *readWriteSocket;

public slots:
    void addOutText(QString);
    void addInText(QString);
    void acceptConnection();
    void recvMessage();
    void initServer();

private slots:
    void SendMove(int,int,int,int,int,int);
    void on_GiveUpBtn_clicked();
};

#endif // MAINWINDOW_H
