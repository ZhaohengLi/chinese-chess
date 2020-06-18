#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

private:
    Ui::ConnectDialog *ui;

private slots:
    void on_YesBtn_clicked();
    void on_RedoBtn_clicked();
    void appendtext(QString);

signals:
    void sendData(QString);
};

#endif // CONNECTDIALOG_H
