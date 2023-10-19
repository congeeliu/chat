#include "sql.h"
#include "util.h"
#include <QDebug>
#include <QSqlQuery>

const QString HOST_NAME = Util::getConfigValue("MYSQL", "ip_address");
const int PORT = Util::getConfigValue("MYSQL", "port").toInt();
const QString USERNAME = Util::getConfigValue("MYSQL", "username");
const QString PASSWORD = Util::getConfigValue("MYSQL", "password");
const QString DATABASE_NAME = Util::getConfigValue("MYSQL", "database_name");

Sql::Sql()
{

}

void Sql::connectDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(HOST_NAME);
    db.setPort(PORT);
    db.setUserName(USERNAME);
    db.setPassword(PASSWORD);
    db.setDatabaseName(DATABASE_NAME);

    if (db.open()) qDebug() << "连接数据库成功";
    else qDebug() << "连接数据库失败";
}

QString Sql::addToFriendList(QString userId, QString friendId)
{
    if (userId == friendId) return "不能添加自己";

    QString check = QString("SELECT COUNT(*) from user where id = '%1';").arg(friendId);
    QSqlQuery query;
    query.exec(check);
    while(query.next())
        if (query.value(0).toString() == "0")
            return "账号不存在";

//    QString sql[2] = {
//        QString("SELECT friend_id FROM friend_list WHERE user_id = '%1';").arg(userId),
//        QString("SELECT user_id FROM friend_list WHERE friend_id = '%1';").arg(userId)
//    };
    QString sql = QString("select friend_id from friend_list where user_id = '%1' union select user_id from friend_list where friend_id = '%2';").arg(userId).arg(userId);
//    for (int i = 0; i < 2; i ++)
//    {
//        query.exec(sql[i]);
//        while(query.next())
//            if (query.value(0).toString() == friendId)
//                return "好友已存在";
//    }
    query.exec(sql);
    while(query.next())
        if (query.value(0).toString() == friendId)
            return "好友已存在";

    QString addFriend = QString("INSERT INTO friend_list (user_id, friend_id) VALUES ('%1', '%2');").arg(userId).arg(friendId);
    if (query.exec(addFriend)) return "success";
    else return "fail";
}

QString Sql::registerAccount(QString account, QString password, QString nickname)
{
    const QString photo = "1.jpg";
    if (account == "") return "账号不能为空";
    if (password == "") return "密码不能为空";
    if (nickname == "") return "昵称不能为空";

    QString find = QString("SELECT COUNT(*) from user WHERE account = '%1';").arg(account);
    QSqlQuery query;
    query.exec(find);
    while(query.next())
        if (query.value(0).toString() != "0")
            return "账号已存在";

    password = Util::MD5(password);
    QString registerAccount = QString("INSERT INTO user (account, password, nickname, photo) VALUES ('%1', '%2', '%3', '%4');").arg(account).arg(password).arg(nickname).arg(photo);
    if (query.exec(registerAccount)) return "success";
    else return "fail";
}

QString Sql::deleteFriendById(QString userId, QString friendId)
{
    QString findId = QString("DELETE FROM friend_list WHERE (user_id = '%1' AND friend_id = '%2') OR (user_id = '%3' AND friend_id = '%4');").arg(userId).arg(friendId).arg(friendId).arg(userId);
    QSqlQuery query;
    if (query.exec(findId)) return "success";
    else return "fail";
}

void Sql::updatePublicKeyById(QString id, std::string pub_e, std::string pub_n)
{
    QString findPublicKey = QString("update public_key SET pub_e = '%1', pub_n = '%2' WHERE user_id = '%3';")
            .arg(QString::fromStdString(pub_e)).arg(QString::fromStdString(pub_n)).arg(id);
    QSqlQuery query;
    query.exec(findPublicKey);
}

User Sql::findUserByAccount(QString account)
{
//    QString sql = QString("select id, nickname, photo from user where account = '%1';").arg(account);
    QSqlQuery query;
    query.prepare("select id, nickname, photo from user where account = ?;");
    query.addBindValue(account);
    query.exec();

    if (query.next())
    {
        QString id = query.value(0).toString();
        QString nickname = query.value(1).toString();
        QString photo = query.value(2).toString();
        User user(id, nickname, photo);
        return user;
    }
//    if (query.next()) return query.value(0).toString();
    else return User();
}

std::pair<std::string, std::string> Sql::findPublicKeyById(QString id)
{
    QString findPublicKey = QString("select pub_e, pub_n from public_key where id = '%1';").arg(id);
    QSqlQuery query;
    query.exec(findPublicKey);
    if (query.next())
        return {query.value(0).toString().toStdString(), query.value(1).toString().toStdString()};
    else
        return {"", ""};
}

QString Sql::findIdByAccount(QString account)
{
    QString findId = QString("select id from user where account = '%1';").arg(account);
    QSqlQuery query;
    query.exec(findId);
    if (query.next()) return query.value(0).toString();
    else return "";
}

QVector<FriendInfo *> Sql::findFriendListById(QString id)
{
//    QString sql[2] = {
//        QString("select friend_id from friend_list where user_id = '%1' union select user_id from friend_list where friend_id = '%2';").arg(id).arg(id),
////        QString("select user_id from friend_list where friend_id = '%1';").arg(id)
//    };
    QString sql = QString("select friend_id from friend_list where user_id = '%1' union select user_id from friend_list where friend_id = '%2';").arg(id).arg(id);
    QSqlQuery query;
    QString friendIdSplitByComma = "";
//    for (int i = 0, cnt = 0; i < 2; i ++)
//    {
//        query.exec(sql[i]);
//        while(query.next())
//        {
//            if (cnt ++) friendIdSplitByComma.append(",");
//            friendIdSplitByComma.append(query.value(0).toString());
//        }
//    }
    query.exec(sql);
    int cnt = 0;
    while(query.next())
    {
        if (cnt ++) friendIdSplitByComma.append(",");
        friendIdSplitByComma.append(query.value(0).toString());
    }

    QVector<FriendInfo *> friendList;

    QString getInfo = QString("select id, nickname, photo from user where id in (%1);").arg(friendIdSplitByComma);
    query.exec(getInfo);
    while(query.next())
    {
        QString id = query.value(0).toString();
        QString nickname = query.value(1).toString();
        QString photo = query.value(2).toString();
        User user(id, nickname, photo);
//        FriendInfo *friendInfo = new FriendInfo(id, nickname, photo);
        FriendInfo *friendInfo = new FriendInfo(user);
        friendList.append(friendInfo);
    }

    return friendList;
}

QString Sql::login(QString account, QString password)
{
    QString userId = findIdByAccount(account);
    if (userId == "") return "用户不存在";

    QString getIdPwd = QString("select password from user where account = '%1';").arg(account);
    QSqlQuery query;
    query.exec(getIdPwd);
    if (query.next())
    {
        QString getPwd = query.value(0).toString();
        if (Util::MD5(password) == getPwd) return "success";
//        if (password == getPwd) return "success";
        else return "密码错误";
    }
    else return "fail";
}

//QString Sql::findNicknameById(QString id)
//{
//    QString findNickname = QString("select nickname from user where id = '%1';").arg(id);
//    QSqlQuery query;
//    query.exec(findNickname);
//    if (query.next()) return query.value(0).toString();
//    else return "";
//}

//QString Sql::findPhotoById(QString id)
//{
//    QString findPhoto = QString("select photo from user where id = '%1';").arg(id);
//    QSqlQuery query;
//    query.exec(findPhoto);
//    if (query.next()) return query.value(0).toString();
//    else return "";
//}
