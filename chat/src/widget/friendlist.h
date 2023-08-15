#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include "chatbox.h"
#include <QWidget>
#include <QListWidgetItem>
#include <QTcpSocket>
#include <QHash>

namespace Ui {
class FriendList;
}

class FriendList : public QWidget
{
    Q_OBJECT

public:
    explicit FriendList(QString _userId, QString _nickname, QString _userPhoto, QWidget *parent = nullptr);
    ~FriendList();

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void updateCheckBoxMessage();

    void on_addButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::FriendList *ui;
    QString userId;
    QString nickname;
    QString userPhoto;
    QTcpSocket *socket;
    QHash<QString, ChatBox *> chatBox;
    QHash<QString, std::string> keys;
    void setFriendList();
    void addItem(QString id, QString nickname, QString photo);
    void connectServer();
};

#endif // FRIENDLIST_H
