#ifndef FRIENDINFO_H
#define FRIENDINFO_H

#include "src/user/user.h"
#include <QWidget>

namespace Ui {
class FriendInfo;
}

class FriendInfo : public QWidget
{
    Q_OBJECT

public:
//    explicit FriendInfo(QString _id, QString _nickname, QString _photo, QWidget *parent = nullptr);
    explicit FriendInfo(User _user, QWidget *parent = nullptr);
    ~FriendInfo();
    QString getId();
    QString getNickname();
    QString getPhoto();

private:
    Ui::FriendInfo *ui;
//    QString id;
//    QString nickname;
//    QString photo;
    User user;
};

#endif // FRIENDINFO_H
