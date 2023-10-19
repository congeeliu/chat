#include "friendinfo.h"
#include "ui_friendinfo.h"
#include <QDebug>
#include <src/utils/util.h>

const QString HEAD_IMAGE_PATH = Util::getConfigValue("IMAGE_PATH", "head_path");

FriendInfo::FriendInfo(User _user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendInfo)
{
    ui->setupUi(this);

    user = _user;

    ui->nickname->setText(user.getNickname());
    QPixmap pix(HEAD_IMAGE_PATH + user.getPhoto());
    ui->photo->setPixmap(pix);
}

FriendInfo::~FriendInfo()
{
    delete ui;
}

QString FriendInfo::getId()
{
    return user.getId();
}

QString FriendInfo::getNickname()
{
    return user.getNickname();
}

QString FriendInfo::getPhoto()
{
    return user.getPhoto();
}
