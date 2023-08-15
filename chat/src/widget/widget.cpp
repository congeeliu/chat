#include "widget.h"
#include "ui_widget.h"
#include "friendlist.h"
#include "src/utils/sql.h"
#include "src/utils/util.h"
#include "register.h"
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    Sql::connectDatabase();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_loginButton_clicked()
{
    QString account = ui->accountLineEdit->text();
    QString password = ui->pwdLineEdit->text();

    QString errorMessage = Sql::login(account, password);
    if (errorMessage == "success")
    {
        QMessageBox::information(this, "登录提示", "登录成功");
        this->hide();
        QString userId = Sql::findIdByAccount(account);
        QString nickname = Sql::findNicknameById(userId);
        QString photo = Sql::findPhotoById(userId);
        FriendList *f = new FriendList(userId, nickname, photo);
        f->show();
    }
    else QMessageBox::warning(this, "登录提示", errorMessage);
}

void Widget::on_registerButton_clicked()
{
    Register *registerWidget = new Register();
    registerWidget->show();
}
