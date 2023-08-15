#include "register.h"
#include "ui_register.h"
#include "src/utils/sql.h"
#include <QMessageBox>

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
}

Register::~Register()
{
    delete ui;
}

void Register::on_registerButton_clicked()
{
    QString account = ui->accountLineEdit->text();
    QString password = ui->pwdLineEdit->text();
    QString nickname = ui->nicknameLineEdit->text();
    QString errorMessage = Sql::registerAccount(account, password, nickname);
    if (errorMessage == "success") QMessageBox::information(this, "注册提示", "注册成功");
    else QMessageBox::warning(this, "注册提示", errorMessage);
}
