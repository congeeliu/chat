#include "user.h"

User::User()
{

}

User::User(QString _id, QString _nickname, QString _photo)
{
    id = _id;
    nickname = _nickname;
    photo = _photo;
}

QString User::getId()
{
    return id;
}

QString User::getNickname()
{
    return nickname;
}

QString User::getPhoto()
{
    return photo;
}
