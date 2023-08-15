#include "friendinfo.h"
#include "ui_friendinfo.h"
#include <QDebug>
#include <src/utils/util.h>

const QString HEAD_IMAGE_PATH = Util::getConfigValue("IMAGE_PATH", "head_path");

FriendInfo::FriendInfo(QString _id, QString _nickname, QString _photo, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendInfo)
{
    ui->setupUi(this);

    id = _id;
    nickname = _nickname;
    photo = _photo;

    ui->nickname->setText(nickname);
    QPixmap pix(HEAD_IMAGE_PATH + photo);
    ui->photo->setPixmap(pix);
}

FriendInfo::~FriendInfo()
{
    delete ui;
}

QString FriendInfo::getId()
{
    return id;
}

QString FriendInfo::getNickname()
{
    return nickname;
}

QString FriendInfo::getPhoto()
{
    return photo;
}
