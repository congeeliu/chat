#ifndef CHATBOX_H
#define CHATBOX_H

#include "src/user/user.h"
#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class ChatBox;
}

class ChatBox : public QWidget
{
    Q_OBJECT

public:
//    explicit ChatBox(QString _userId, QString _userNickname, QString _friendId, QString _friendNickname, QTcpSocket *_socket, QWidget *parent = nullptr);
    explicit ChatBox(User _user, User _friend, QTcpSocket *_socket, QWidget *parent = nullptr);
    ~ChatBox();
    void setFriendPublicKey(std::string _friend_pub_e, std::string _friend_pub_n);
    void setPrivateKey(std::string _pri_d, std::string _pri_n);
    void updateMessage(QString receiveMessage);

private slots:
    void on_closeButton_clicked();
    void on_sendButton_clicked();

private:
    Ui::ChatBox *ui;
//    QString userId;
//    QString userNickname;
    User user;
    User curFriend;
//    QString friendId;
//    QString friendNickname;
    QTcpSocket *socket;
    std::string friend_pub_e, friend_pub_n, pri_d, pri_n;
};

#endif // CHATBOX_H
