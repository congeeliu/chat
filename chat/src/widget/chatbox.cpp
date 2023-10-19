#include "chatbox.h"
#include "ui_chatbox.h"
#include "src/utils/util.h"
#include "src/utils/rsa.h"
#include <QDebug>
#include <QCryptographicHash>  // md5
#include <QMessageBox>

const QString FUNCTIONAL_BLOCK_IMAGE_PATH = Util::getConfigValue("IMAGE_PATH", "functional_block");

ChatBox::ChatBox(User _user, User _friend, QTcpSocket *_socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatBox)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时释放内存

    user = _user;
    curFriend = _friend;
    socket = _socket;

    QPixmap pix(FUNCTIONAL_BLOCK_IMAGE_PATH);
    ui->functionalBlock->setPixmap(pix);
    setWindowTitle(curFriend.getNickname());

//    connect(socket, &QTcpSocket::readyRead, this, &ChatBox::updateMessage);
}

ChatBox::~ChatBox()
{
    delete ui;
}

void ChatBox::setFriendPublicKey(std::string _friend_pub_e, std::string _friend_pub_n)
{
    friend_pub_e = _friend_pub_e;
    friend_pub_n = _friend_pub_n;
}

void ChatBox::setPrivateKey(std::string _pri_d, std::string _pri_n)
{
    pri_d = _pri_d;
    pri_n = _pri_n;
}

void ChatBox::on_closeButton_clicked()
{
    this->close();
}

void ChatBox::on_sendButton_clicked()
{
    QString content = ui->textEdit->toPlainText();
    ui->textEdit->clear();
    ui->textBrowser->insertPlainText(user.getNickname() + ": " + content + "\n");

    QString ciphertext = Util::encrypt(content, user, curFriend, friend_pub_e, friend_pub_n);

    QByteArray ba;
    ba.append(ciphertext);
    socket->write(ba);
}

void ChatBox::updateMessage(QString receiveMessage)
{
    pair<bool, QString> p = Util::decrypt(receiveMessage, pri_d, pri_n);

    if (p.first) ui->textBrowser->insertPlainText(curFriend.getNickname() + ": " + p.second + "\n");
    else
    {
        ui->textBrowser->insertPlainText("!!已屏蔽该消息\n");
        QMessageBox::warning(this, "警告", "对方消息已被篡改");
    }
}
