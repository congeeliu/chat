#include "deletefriend.h"
#include "ui_deletefriend.h"
#include "src/utils/sql.h"
#include <QDebug>
#include <QMessageBox>

DeleteFriend::DeleteFriend(QString _userId, QString _friendId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeleteFriend)
{
    ui->setupUi(this);

    userId = _userId;
    friendId = _friendId;
}

DeleteFriend::~DeleteFriend()
{
    delete ui;
}

void DeleteFriend::on_trueButton_clicked()
{
    QString errorMessage = Sql::deleteFriendById(userId, friendId);
    if (errorMessage == "success")
    {
        emit deleteFriendSuccess();
        QMessageBox::information(this, "删除提示", "删除好友成功");
        this->close();
    }
    else
    {
        QMessageBox::warning(this, "删除提示", errorMessage);
    }
}

void DeleteFriend::on_falseButton_clicked()
{
    this->close();
}
