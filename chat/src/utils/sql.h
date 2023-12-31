#ifndef SQL_H
#define SQL_H

#include "src/widget/friendinfo.h"
#include "src/user/user.h"
#include <QSqlDatabase>
#include <QVector>

class Sql
{
public:
    Sql();
    static void connectDatabase();
    static QString addToFriendList(QString userId, QString friendId);
    static QString registerAccount(QString account, QString password, QString nickname);
    static QString deleteFriendById(QString userId, QString friendId);
    static void updatePublicKeyById(QString id, std::string pub_e, std::string pub_n);
    static User findUserByAccount(QString account);
    static std::pair<std::string, std::string> findPublicKeyById(QString id);
    static QString findIdByAccount(QString account);
    static QVector<FriendInfo *> findFriendListById(QString id);
    static QString login(QString account, QString password);
//    static QString findNicknameById(QString id);
//    static QString findPhotoById(QString id);

private:
//    QSqlDatabase db;
};

#endif // SQL_H
