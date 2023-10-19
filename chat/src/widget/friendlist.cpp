#include "friendlist.h"
#include "ui_friendlist.h"
#include "friendinfo.h"
#include "src/utils/util.h"
#include "src/utils/sql.h"
#include "addfriend.h"
#include "deletefriend.h"
#include <QSqlQuery>
#include <QDebug>
#include <QVector>
#include <QHostAddress>
#include <QHash>
#include <QMessageBox>

const QString IP_ADDRESS = Util::getConfigValue("TCP_SERVER", "ip_address");
const QString PORT = Util::getConfigValue("TCP_SERVER", "port");
const QString CARD_IMAGE_PATH = Util::getConfigValue("IMAGE_PATH", "card");
const QString BLUE_BACKGROUND_IMAGE_PATH = Util::getConfigValue("IMAGE_PATH", "blue_background");
const QString HEAD_IMAGE_PATH = Util::getConfigValue("IMAGE_PATH", "head_path");

FriendList::FriendList(User _user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendList)
{
    ui->setupUi(this);

    user = _user;

    keys = Util::getRSAKeys();
    Sql::updatePublicKeyById(user.getId(), keys["pub_e"], keys["pub_n"]);

    setHeaderCard();
    setFriendList();

    socket = new QTcpSocket;    // 创建socket对象
    connectServer();

    connect(socket, &QTcpSocket::readyRead, this, &FriendList::updateCheckBoxMessage);
}

FriendList::~FriendList()
{
    delete ui;
}

void FriendList::setHeaderCard()
{
    QPixmap card(":/image/resources/image/resources/card.jpg");
    QPixmap photo(HEAD_IMAGE_PATH + user.getPhoto());
    QPixmap smallCard(BLUE_BACKGROUND_IMAGE_PATH);
    ui->card->setPixmap(card);
    ui->photo->setPixmap(photo);
    ui->smallCard->setPixmap(smallCard);
    ui->nicknameLabel->setText(user.getNickname());
}

void FriendList::setFriendList()
{
    ui->listWidget->clear();
    QVector<FriendInfo *> friendList = Sql::findFriendListById(user.getId());
    for (int i = 0; i < friendList.size(); i ++)
    {
        QListWidgetItem* pItem = new QListWidgetItem();
        pItem->setSizeHint(QSize(208, 41));
        ui->listWidget->addItem(pItem);
        ui->listWidget->setItemWidget(pItem, friendList[i]);
    }
}

void FriendList::on_listWidget_itemDoubleClicked(QListWidgetItem *item) // 打开ChatBox
{
    FriendInfo *friendInfo = static_cast<FriendInfo *>(ui->listWidget->itemWidget(item));
    QString friendId = friendInfo->getId();
//    ChatBox *c = new ChatBox(userId, nickname, friendId, friendInfo->getNickname(), socket);
    User curFriend(friendId, friendInfo->getNickname(), friendInfo->getPhoto());
    ChatBox *c = new ChatBox(user, curFriend, socket);
    chatBox[friendId] = c;
    c->setPrivateKey(keys["pri_d"], keys["pri_n"]);
    std::pair<std::string, std::string> publicKey = Sql::findPublicKeyById(friendId);
    c->setFriendPublicKey(publicKey.first, publicKey.second);

    c->show();
}

void FriendList::connectServer()
{
    // 连接服务器
    socket->connectToHost(QHostAddress(IP_ADDRESS), PORT.toUShort());

    // 连接服务器成功，socket对象会发出信号
    connect(socket, &QTcpSocket::connected, [this]()
    {
//        QMessageBox::information(this, "连接提示", "连接服务器成功");
        qDebug() << "连接服务器成功";
        QByteArray ba;
        ba.append(user.getId());
        socket->write(ba);
    });

    // 连接断开，socket会发出信号
    connect(socket, &QTcpSocket::disconnected, []()
    {
//        QMessageBox::warning(this, "连接提示", "连接异常，与服务器断开连接");
        qDebug() << "连接异常，与服务器断开连接";
    });
}

void FriendList::updateCheckBoxMessage()
{
    QString receiveMessage = QString(socket->readAll());
    QString friendId;
    for (int i = 0; i < receiveMessage.size(); i ++)
        if (receiveMessage[i] == ':')
        {
            friendId = receiveMessage.mid(0, i);
            receiveMessage = receiveMessage.mid(i + 1);
            break;
        }
    chatBox[friendId]->updateMessage(receiveMessage);
}

void FriendList::on_addButton_clicked()
{
    AddFriend *addFriend = new AddFriend(user.getId());
    addFriend->show();
    connect(addFriend, &AddFriend::addFriendSuccess, this, &FriendList::setFriendList);
}

void FriendList::on_deleteButton_clicked()
{
    QList<QListWidgetItem *> items = ui->listWidget->selectedItems();
    if (items.size())
    {
        FriendInfo *friendInfo = static_cast<FriendInfo *>(ui->listWidget->itemWidget(items[0]));
        DeleteFriend *deleteFriend = new DeleteFriend(user.getId(), friendInfo->getId());
        deleteFriend->show();
        connect(deleteFriend, &DeleteFriend::deleteFriendSuccess, this, &FriendList::setFriendList);
    }
    else QMessageBox::warning(this, "删除提示", "未选中要删除的好友");
}
