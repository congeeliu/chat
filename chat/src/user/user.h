#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();
    User(QString _id, QString _nickname, QString _photo);
    QString getId();
    QString getNickname();
    QString getPhoto();

private:
    QString id;
    QString account;
    QString nickname;
    QString photo;
};

#endif // USER_H
