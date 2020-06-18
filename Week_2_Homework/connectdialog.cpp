#include "connectdialog.h"
#include "ui_connectdialog.h"
#include <QSignalMapper>
#include <QString>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
    QSignalMapper *mapper = new QSignalMapper(this);

    for(int i=0; i<10; i++){
        QPushButton *pBtn = new QPushButton(QString::number(i),this);
        connect(pBtn, SIGNAL(clicked()), mapper, SLOT(map()));
        mapper->setMapping(pBtn, pBtn->text());
        ui->gridLayout_2->addWidget(pBtn,i,0);
    }
    QPushButton *pBtn = new QPushButton(".",this);
    connect(pBtn, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(pBtn, pBtn->text());
    ui->gridLayout_2->addWidget(pBtn,10,0);
    connect(mapper,SIGNAL(mapped(QString)),this,SLOT(appendtext(QString)));
}

ConnectDialog::~ConnectDialog()
{
    delete ui;

}

void ConnectDialog::on_YesBtn_clicked(){
    QString IP = ui->IPLineEdit->text();
    this->close();
    emit sendData(IP);
}

void ConnectDialog::on_RedoBtn_clicked(){
    ui->IPLineEdit->clear();
}
void ConnectDialog::appendtext(QString a){
    QString temp = ui->IPLineEdit->text()+a;
    ui->IPLineEdit->setText(temp);
    repaint();
}
