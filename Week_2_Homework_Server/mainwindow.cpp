#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QDebug>
#include <QDateTime>
#include <QString>
#include <QMessageBox>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QProcess>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QAction *Start = new QAction(this);
    Start->setIconText("开放连接端口");
    connect(Start,SIGNAL(triggered(bool)),this,SLOT(initServer()));
    ui->mainToolBar->addAction(Start);
    QMenu *Init = ui->menuBar->addMenu("开始");
    Init->addAction(Start);

    setWindowTitle("服务器端，红方");
    connect(ui->widget,SIGNAL(move(int,int,int,int,int,int)),this,SLOT(SendMove(int,int,int,int,int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initServer(){
    //qDebug()<<"from func initServer in MainWindow";
    this->listenSocket =new QTcpServer;
    this->listenSocket->listen(QHostAddress::Any,8989);
    addInText("已在 8989 端口开始监听连接");
    QObject::connect(this->listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    QString ipaddr = getIP(QHostInfo::localHostName());
    if(QMessageBox::No == QMessageBox::question(this,"已经开始监听连接","本机IP地址为"+ipaddr+"\n已经开始监听连接，你可以选择是否继续等待客户端的连接\n选择“No”将不再等待",
                                                QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes)){
        listenSocket->close();
        addInText("不再等待连接，连接失败");
    }
}

void MainWindow::on_GiveUpBtn_clicked(){
    QByteArray *array =new QByteArray;
    array->clear();
    array->append("surrender");
    this->readWriteSocket->write(array->data());
    addOutText(array->data());
    this->repaint();
    ui->widget->Game_Is_Over=true;
    QMessageBox::information(this,"您输了","您已投降，对方获胜了！");

}

void MainWindow::SendMove(int Eat_x, int Eat_y, int Eat_value, int Eaten_x, int Eaten_y, int Eaten_value){
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(QString::number(Eat_x)+","+QString::number(Eat_y)+","+QString::number(Eat_value)+","+QString::number(Eaten_x)+","+QString::number(Eaten_y)+","+QString::number(Eaten_value));
    this->readWriteSocket->write(array->data());
    addOutText(array->data());
    this->repaint();
}

void MainWindow::addInText(QString _InText){
    QDateTime ct = QDateTime::currentDateTime();
    QString time=ct.toString();
    ui->InTextEdit->append("-----------------------------\n"+time+"\n"+_InText+"\n-----------------------------\n");
}

void MainWindow::addOutText(QString _OutText){
    QDateTime ct = QDateTime::currentDateTime();
    QString time=ct.toString();
    ui->OutTextEdit->append("-----------------------------\n"+time+"\n"+_OutText+"\n-----------------------------\n");
}

void MainWindow::acceptConnection(){
    this->readWriteSocket =this->listenSocket->nextPendingConnection();
    addInText("连接已建立");
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
}

void MainWindow::recvMessage(){
    bool handeled = false;
    QString info;
    info += this->readWriteSocket->readAll();
    if(info == "start"){
        qDebug()<<"recvMessage start";
        ui->widget->Start_Game->click();
        handeled = true;
    }
    if(info == "surrender"){
        qDebug()<<"recvMessage surrender";
        QMessageBox::information(this,"您赢了","对方已投降，您获胜了！");
        ui->widget->Game_Is_Over=true;
        handeled = true;
    }
    if(info == "readgame"){
        qDebug()<<"recvMessage readgame";
        ui->widget->Read_Game_Slots();
        handeled = true;
    }
    if(info == "savegame"){
        qDebug()<<"recvMessage savegame";
        ui->widget->Save_Game_Slots();
        handeled = true;
    }
    if(!handeled){
        QStringList list = info.split(",");
        ui->widget->Eat_Chessman(list[0].toInt(),list[1].toInt(),list[2].toInt(),list[3].toInt(),list[4].toInt(),list[5].toInt());
    }
    addInText(info);
}

QString MainWindow::getIP(QString localHost)
{
    QString ipAddr;
#if 0
    QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();
    foreach(QHostAddress address, AddressList){
        if(address.protocol() == QAbstractSocket::IPv4Protocol &&
                address != QHostAddress::Null &&
                address != QHostAddress::LocalHost){
            if (address.toString().contains("127.0.")){
                continue;
            }
            ipAddr = address.toString();
            break;
        }
    }
#else
    QHostInfo info = QHostInfo::fromName(localHost);
    info.addresses();//QHostInfo的address函数获取本机ip地址
    //如果存在多条ip地址ipv4和ipv6：
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol()==QAbstractSocket::IPv4Protocol){//只取ipv4协议的地址
            qDebug()<<address.toString();
            ipAddr = address.toString();
        }
    }
#endif
    return ipAddr;
}
