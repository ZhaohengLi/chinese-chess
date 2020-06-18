#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"connectdialog.h"

#include <QAction>
#include <QDebug>
#include <QDateTime>
#include <QString>
#include <QMessageBox>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QAction *Start = new QAction(this);
    Start->setIconText("建立连接");
    connect(Start,SIGNAL(triggered(bool)),this,SLOT(callConnectDialog()));
    ui->mainToolBar->addAction(Start);
    QMenu *Init = ui->menuBar->addMenu("开始");
    Init->addAction(Start);

    setWindowTitle("客户端,黑方");
    connect(ui->widget,SIGNAL(move(int,int,int,int,int,int)),this,SLOT(SendMove(int,int,int,int,int,int)));
    connect(ui->widget,SIGNAL(startgame()),this,SLOT(sendstart()));
    connect(ui->widget,SIGNAL(readgame(QString)),this,SLOT(sendreadgame(QString)));
    connect(ui->widget,SIGNAL(savegame()),this,SLOT(sendsavegame()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::callConnectDialog(){
    ConnectDialog *cd = new ConnectDialog(this);
    connect(cd,SIGNAL(sendData(QString)),this,SLOT(startConnection(QString)));
    cd->exec();
}

void MainWindow::startConnection(QString ip){
    ReadWriteSocket = new QTcpSocket(this);
    //ReadWriteSocket->connectToHost(QHostAddress(a),b);
    //ReadWriteSocket->connectToHost(QHostAddress("192.168.0.103"),8989);
    ReadWriteSocket->connectToHost(QHostAddress(ip),8989);
    addInText("尝试建立连接 "+ip+" 8989");
    if (ReadWriteSocket->waitForConnected(600000) == false){
        if (QMessageBox::No == QMessageBox::question(this,
                                                      tr("已经发起连接请求"),
                                                      tr("当前连接无响应，是否继续等待连接"),
                                                      QMessageBox::Yes | QMessageBox::No,
                                                      QMessageBox::Yes)) {
            ReadWriteSocket->close();
            addInText("不再继续等待连接,连接失败");
        }
    } else {
        addInText("连接成功");
        QObject::connect(ReadWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
    }
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

void MainWindow::recvMessage(){
    QString info;
    info += this->ReadWriteSocket->readAll();
    if(info == "surrender"){
        qDebug()<<"recvMessage surrender";
        QMessageBox::information(this,"您赢了","对方已投降，您获胜了！");
        ui->widget->Game_Is_Over=true;
    } else {
        QStringList list = info.split(",");
        ui->widget->Eat_Chessman(list[0].toInt(),list[1].toInt(),list[2].toInt(),list[3].toInt(),list[4].toInt(),list[5].toInt());
    }
    addInText(info);
}

void MainWindow::SendMove(int Eat_x, int Eat_y, int Eat_value, int Eaten_x, int Eaten_y, int Eaten_value){
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(QString::number(Eat_x)+","+QString::number(Eat_y)+","+QString::number(Eat_value)+","+QString::number(Eaten_x)+","+QString::number(Eaten_y)+","+QString::number(Eaten_value));
    this->ReadWriteSocket->write(array->data());
    addOutText(array->data());
    this->repaint();
}

void MainWindow::sendstart(){
    QByteArray *array =new QByteArray;
    array->clear();
    array->append("start");
    this->ReadWriteSocket->write(array->data());
    addOutText(array->data());
    this->repaint();
}

void MainWindow::sendreadgame(QString filename){
    QByteArray *array =new QByteArray;
    array->clear();
    array->append("readgame");
    this->ReadWriteSocket->write(array->data());
    addOutText(array->data());
    this->repaint();
}
void MainWindow::sendsavegame(){
    QByteArray *array =new QByteArray;
    array->clear();
    array->append("savegame");
    this->ReadWriteSocket->write(array->data());
    addOutText(array->data());
    this->repaint();
}

void MainWindow::on_GiveUpBtn_clicked(){
    QByteArray *array =new QByteArray;
    array->clear();
    array->append("surrender");
    this->ReadWriteSocket->write(array->data());
    addOutText(array->data());
    this->repaint();
    ui->widget->Game_Is_Over=true;
    QMessageBox::information(this,"您输了","您已投降，对方获胜了！");

}








