#include "widget.h"
#include "ui_widget.h"
#include "friendlist.h"
#include "src/utils/sql.h"
#include "src/utils/util.h"
#include "register.h"
#include "src/user/user.h"
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
        User user = Sql::findUserByAccount(account);
        FriendList *f = new FriendList(user);
        f->show();
        this->hide();
    }
    else QMessageBox::warning(this, "登录提示", errorMessage);
}

void Widget::on_registerButton_clicked()
{
    Register *registerWidget = new Register();
    registerWidget->show();
}
