#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

const int PORT = 8000;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    server = new QTcpServer;

    if (!server->listen(QHostAddress::AnyIPv4, PORT))
    {
        qDebug() << server->errorString();
        close();
    }
//    server->listen(QHostAddress::AnyIPv4, PORT);

    // 客户端发起连接，server发出信号
    connect(server, &QTcpServer::newConnection, this, &Widget::newClientHandler);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::updateLoginTextBrowser()
{
    QString content = "";
    for (QString s: loginList)
    {
        content.append(s + "\n");
    }
    ui->loginTextBrowser->setText(content);
}

void Widget::newClientHandler()
{
    // 建立TCP连接
    QTcpSocket *socket = server->nextPendingConnection();

    QString ip = socket->peerAddress().toString();
    QString port = QString::number(socket->peerPort());
    ui->messageTextBrowser->insertPlainText(QString("客户端[%1:%2]成功连接\n").arg(ip).arg(port));
    loginList.push_back(ip + ":" + port);
    updateLoginTextBrowser();

    isFirst[socket] = true;

    //断开连接
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

//    // 服务器收到客户端发送的消息，socket发出readyread信号
    connect(socket, &QTcpSocket::readyRead, this, &Widget::sendMessage);

    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(onDisplayError(QAbstractSocket::SocketError)));
//    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(MSGError(QAbstractSocket::SocketError)));
}

void Widget::sendMessage()
{
    // 获取信号的发出者
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
//    QTcpSocket *socket = (QTcpSocket *)sender();
    QString content = QString(socket->readAll());

    if (isFirst[socket])
    {
        isFirst[socket] = false;
        QString userId = content;
        sockets[userId] = socket;
        return;
    }

    QString userId1, userId2;
    for (int i = 0, idx = 0; i < content.size(); i ++)
    {
        if (content[i] == ':')
        {
            userId2 = content.mid(idx, i - idx);
            content = content.mid(i + 1);
            break;
        }
        if (content[i] == ',')
        {
            userId1 = content.mid(0, i);
            idx = i + 1;
        }
    }

    QByteArray ba;
    ba.append(userId1 + ":" + content);
//    content = content.mid(0, content.size() - 34) + "1" + content.mid(content.size() - 34, 34);
//    qDebug() << content;
//    ba.append(content);
    if (sockets[userId2] != nullptr) sockets[userId2]->write(ba);
    else qDebug() << "user whose Id is " + userId2 + " is not logged in";
}

void Widget::onDisplayError(QAbstractSocket::SocketError)
{
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
    QString ip = socket->peerAddress().toString();
    QString port = QString::number(socket->peerPort());

    for (int i = 0; i < loginList.size(); i ++)
        if (loginList[i] == ip + ":" + port)
        {
            loginList.erase(loginList.begin() + i);
            break;
        }
    updateLoginTextBrowser();

    int error = socket->error();
    switch(error)
    {
        case QAbstractSocket::RemoteHostClosedError://客户端断开
        {
            ui->messageTextBrowser->insertPlainText(QString("客户端[%1:%2]断开连接\n").arg(ip).arg(port));
            break;
        }
        default:
        {
            error = -1;
            qDebug() << socket->errorString();
            break;
        }
    }
}
