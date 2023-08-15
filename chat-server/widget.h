#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QHash>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void newClientHandler();
    void sendMessage();
    void onDisplayError(QAbstractSocket::SocketError);

private:
    Ui::Widget *ui;
    QTcpServer *server;
    QVector<QString> loginList;
    QHash<QString, QTcpSocket *> sockets;
    QHash<QTcpSocket *, bool> isFirst;
    void updateLoginTextBrowser();
};

#endif // WIDGET_H
