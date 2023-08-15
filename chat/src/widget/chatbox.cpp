#include "chatbox.h"
#include "ui_chatbox.h"
#include "src/utils/util.h"
#include "src/utils/rsa.h"
#include <QDebug>
#include <QCryptographicHash>  // md5
#include <QMessageBox>

const QString FUNCTIONAL_BLOCK_IMAGE_PATH = Util::getConfigValue("IMAGE_PATH", "functional_block");

ChatBox::ChatBox(QString _userId, QString _userNickname, QString _friendId, QString _friendNickname, QTcpSocket *_socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatBox)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时释放内存

    userId = _userId;
    userNickname = _userNickname;
    friendId = _friendId;
    friendNickname = _friendNickname;
    socket = _socket;

    QPixmap pix(FUNCTIONAL_BLOCK_IMAGE_PATH);
    ui->functionalBlock->setPixmap(pix);
    setWindowTitle(friendNickname);

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
    QString content =  ui->textEdit->toPlainText();
    ui->textEdit->clear();
    ui->textBrowser->insertPlainText(userNickname + ": " + content + "\n");

    content += "[" + Util::MD5(content) + "]";

    QString AESKey = Util::getRandomKey(16);

    QString ciphertext = Util::AESEncrypt(AESKey, content);

    QString header = userId + "," + friendId + ":";
    QString RSAEncryptAESKey = Util::RSAEncrypt(friend_pub_e, friend_pub_n, AESKey);
    header += QString::number(RSAEncryptAESKey.size()) + ":" + RSAEncryptAESKey;

    QByteArray ba;
    ba.append(header);
    ba.append(ciphertext);
    socket->write(ba);
}

void ChatBox::updateMessage(QString receiveMessage)
{
    int len = 0;
    for (int i = 0; i < receiveMessage.size(); i ++)
    {
        if (receiveMessage[i] == ':')
        {
            len = receiveMessage.mid(0, i).toInt();
            receiveMessage = receiveMessage.mid(i + 1);
            break;
        }
    }

    QString RSAEncryptAESKey = receiveMessage.mid(0, len);
    QString AESKey = Util::RSADecrypt(pri_d, pri_n, RSAEncryptAESKey);
    QString ciphertext = receiveMessage.mid(len);
    QString message = Util::AESDecrypt(AESKey, ciphertext);

    qDebug() << message;

    QString content = message.mid(0, message.size() - 34);
    QString md5 = message.mid(content.size() + 1, 32);
    if (Util::MD5(content) == md5) ui->textBrowser->insertPlainText(friendNickname + ": " + content + "\n");
    else
    {
        qDebug() << message;
        qDebug() << Util::MD5(content) << md5;
        ui->textBrowser->insertPlainText("!!已屏蔽该消息\n");
        QMessageBox::warning(this, "警告", "对方消息已被篡改");
    }
}
