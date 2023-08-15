#include "addfriend.h"
#include "ui_addfriend.h"
#include "src/utils/sql.h"
#include <QDebug>
#include <QMessageBox>

AddFriend::AddFriend(QString _userId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFriend)
{
    ui->setupUi(this);

    userId = _userId;
}

AddFriend::~AddFriend()
{
    delete ui;
}

void AddFriend::on_addButton_clicked()
{
    QString account = ui->accountLineEdit->text();
    QString friendId = Sql::findIdByAccount(account);
    QString errorMessage = Sql::addToFriendList(userId, friendId);
    if (errorMessage == "success")
    {
        emit addFriendSuccess();
        QMessageBox::information(this, "添加提示", "添加好友成功");
    }
    else
    {
        QMessageBox::warning(this, "添加提示", errorMessage);
    }
}
