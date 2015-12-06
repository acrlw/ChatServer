#ifndef CHATSERVERWIDGET_H
#define CHATSERVERWIDGET_H

#include <QWidget>
#include <QtNetwork>
#include <QThread>
#include <QDateTime>
#include <QList>

namespace Ui {
class ChatServerWidget;
}

class ChatServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatServerWidget(QWidget *parent = 0);
    ~ChatServerWidget();

    void sendToSocket(QTcpSocket *socket, QString str);
    void deleteSocket(QTcpSocket *socket);
    void updateClient();
    void closeAllClient();
public slots:
    void deleteUserInfo();
    void onNewConnection();
    void handleError();
    void readMessage();
    void serverError();

private slots:
    void on_pushButton_clicked();


    void on_checkBox_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::ChatServerWidget *ui;
    QTcpServer * server;
    QList<QTcpSocket*> lstSocket;
    QString ver = "1.0";
};

#endif // CHATSERVERWIDGET_H
