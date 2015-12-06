#include "chatserverwidget.h"
#include "ui_chatserverwidget.h"

ChatServerWidget::ChatServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatServerWidget)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    qDebug() << "server listen result:" << server->listen(QHostAddress("127.0.0.1"),2333);
    connect(server,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
    connect(server,SIGNAL(acceptError(QAbstractSocket::SocketError)),this,SLOT(serverError()));
}

void ChatServerWidget::onNewConnection()
{
    while(server->hasPendingConnections())
    {
        qDebug() << "oh!The server has a new connection!.";
        QString str = "sn3:new";
        QTcpSocket * info = server->nextPendingConnection();
        connect(info,SIGNAL(disconnected()),this,SLOT(deleteUserInfo()));
        connect(info,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(handleError()));
        connect(info,SIGNAL(readyRead()),this,SLOT(readMessage()));
        foreach (QTcpSocket * sock, lstSocket)
            sock->write(str.toUtf8());
        lstSocket.append(info);
    }
}

void ChatServerWidget::serverError()
{
    qDebug() << "server error:" << server->errorString();
}

void ChatServerWidget::handleError()
{
    QTcpSocket * socket = qobject_cast<QTcpSocket*>(sender());
    qDebug() << socket->errorString();
}
void ChatServerWidget::readMessage()
{
    QTcpSocket * socket = qobject_cast<QTcpSocket*>(sender());
    QString cmd = socket->readAll();                                        //截获命令
    if(QString::compare(cmd.left(2),"sm") == 0)
        return;
    while(!cmd.isEmpty())                                                   //解决粘包问题
    {
        QString msgContext = cmd.mid(cmd.indexOf(":") + 1,cmd.mid(2,cmd.indexOf(":") - 2).toInt());
        QDateTime time = QDateTime::currentDateTime();                      //获取系统现在的时间
        QString current_date = time.toString("yyyy-MM-dd hh:mm:ss");        //设置显示格式
        if(QString::compare(cmd.left(2),"sg") == 0)
        {
            qDebug() << socket->socketDescriptor() << ":" << msgContext;
            QString str = "rg"+ QString::number(msgContext.length()) + ":" + msgContext;
            foreach (QTcpSocket * sock, lstSocket)
                if(sock->socketDescriptor() != socket->socketDescriptor())
                    sock->write(str.toUtf8());
        }
        if(QString::compare(cmd.left(2),"vv") == 0)
        {
            QString str;
            if(QString::compare(msgContext,ver) != 0)
                str = "fv1:1";
            if(QString::compare(msgContext,ver) == 0)
                str = "sv1:1";
            socket->write(str.toUtf8());
        }
        int cmdLen = 3 + cmd.mid(2,cmd.indexOf(":") - 2).length() + msgContext.length();
        cmd = cmd.remove(0,cmdLen);
    }
}

void ChatServerWidget::deleteSocket(QTcpSocket * socket)
{
    for(int i=0;i<lstSocket.count();i++)
    {
        if(lstSocket[i]->socketDescriptor() == socket->socketDescriptor())
        {
            lstSocket[i]->abort();
            lstSocket[i]->close();
            lstSocket[i]->deleteLater();
            lstSocket.removeAt(i);
        }
    }
}

void ChatServerWidget::deleteUserInfo()
{
    deleteSocket(qobject_cast<QTcpSocket*>(sender()));
    QString str = "sr1:1";
    foreach (QTcpSocket * sock, lstSocket)
        sock->write(str.toUtf8());
}

ChatServerWidget::~ChatServerWidget()
{
    delete ui;
}
void ChatServerWidget::on_pushButton_clicked()
{
    QString str = "ss"+ QString::number(ui->txtMessage->text().length()) +":" + ui->txtMessage->text();
    foreach (QTcpSocket * sock, lstSocket)
        sock->write(str.toUtf8());
}
void ChatServerWidget::updateClient()
{
    QString str = "up1:1";
    foreach (QTcpSocket * sock, lstSocket)
        sock->write(str.toUtf8());
}
void ChatServerWidget::closeAllClient()
{
    QString str = "cl1:1";
    foreach (QTcpSocket * sock, lstSocket)
        sock->write(str.toUtf8());
}

void ChatServerWidget::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked())
        updateClient();
}

void ChatServerWidget::on_pushButton_2_clicked()
{
    closeAllClient();
}

void ChatServerWidget::on_pushButton_3_clicked()
{
    ver = ui->lineEdit->text();
}

void ChatServerWidget::on_pushButton_4_clicked()
{
    QString str = "ga"+ QString::number(ui->txtGagTime->text().length()) +":" + ui->txtGagTime->text();
    QString str1 = "gg1:1";
    foreach (QTcpSocket * sock, lstSocket)
    {
        if(sock->socketDescriptor() == ui->txtDes->text().toInt())
            sock->write(str.toUtf8());
        if(sock->socketDescriptor() != ui->txtDes->text().toInt())
            sock->write(str1.toUtf8());
    }
}

void ChatServerWidget::on_pushButton_5_clicked()
{
    ui->lblOnlineNumber->setText("在线人数:" + QString::number(lstSocket.count()));
    QStringList lst;
    foreach (QTcpSocket * sock, lstSocket)
        lst.append(QString::number(sock->socketDescriptor()));
    ui->lstOnline->clear();
    ui->lstOnline->addItems(lst);
}
