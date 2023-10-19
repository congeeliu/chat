#include "util.h"
#include "rsa.h"
#include "qaesencryption.h"
#include <QCryptographicHash>
#include <QTextCodec>
#include <QDateTime>
#include <QDebug>
#include <QSettings>

const QString CONFIG_PATH = ":/config/resources/config/config.ini";


Util::Util()
{

}

QString Util::getConfigValue(QString group, QString key)
{
    qDebug() << CONFIG_PATH;
    //Read
    QSettings *settings = new QSettings(CONFIG_PATH, QSettings::IniFormat);
    settings->beginGroup(group);
    QString res = settings->value(key, "null").toString();
    settings->endGroup();
    return res;

}

QString Util::MD5(QString s)
{
    QString md5;
    QByteArray str;
    str = QCryptographicHash::hash(s.toLatin1(), QCryptographicHash::Md5);
    md5.append(str.toHex());
    return md5;
}

QString Util::strToHexStr(QString s)
{
    QByteArray hexBa;
    QString hexStr = hexBa.append(s).toHex();
    return hexStr;
}

QString Util::hexStrToStr(QString hexStr)
{
    QByteArray hexBa;
    hexBa.append(hexStr);
    QByteArray str = QByteArray::fromHex(hexBa);
    return QString(str);
}

QHash<QString, std::string> Util::getRSAKeys()
{
    RSA rsa;
    std::string pub_e, pub_n, pri_d, pri_n;

    rsa.generateKey(4096, 16);
    rsa.getKey(RSA::PublicKey, pub_e, pub_n);
    rsa.getKey(RSA::PrivateKey, pri_d, pri_n);

    QHash<QString, std::string> keys;
    keys.insert("pub_e", pub_e);
    keys.insert("pub_n", pub_n);
    keys.insert("pri_d", pri_d);
    keys.insert("pri_n", pri_n);

    return keys;
}

QString Util::RSAEncrypt(std::string pub_e, std::string pub_n, QString text)
{
    RSA rsa;
    rsa.setKey(RSA::PublicKey, pub_e, pub_n);

    QString hexStr = strToHexStr(text);
    std::string ciphertext = rsa.Encrypt(hexStr.toStdString(), 16);

    return QString::fromStdString(ciphertext);
}

QString Util::RSADecrypt(std::string pri_d, std::string pri_n, QString ciphertext)
{
    RSA rsa;
    rsa.setKey(RSA::PrivateKey, pri_d, pri_n);

    std::string plaintext = rsa.Decrypt(ciphertext.toStdString().c_str(), 16);
    QString hexStr = QString::fromStdString(plaintext);
    QString getText = hexStrToStr(hexStr);

    return getText;
}

QString Util::AESEncrypt(QString key, QString text)
{
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::PKCS7);
    QByteArray enBA = encryption.encode(text.toUtf8(), key.toUtf8());
    QString hexStr = QString::fromLatin1(enBA.toHex());
    return hexStr;
}

QString Util::AESDecrypt(QString key, QString hexStr)
{
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::PKCS7);
    QByteArray enBa = QByteArray::fromHex(hexStr.toLatin1());
    QByteArray deBa = encryption.decode(enBa, key.toUtf8());
    QByteArray utf8Ba =  QAESEncryption::RemovePadding(deBa, QAESEncryption::PKCS7); // 移除填充
    QString text = QTextCodec::codecForName("UTF-8")->toUnicode(utf8Ba);
    return text;
}

QString Util::getRandomKey(int len)
{
    qsrand(uint(QDateTime::currentMSecsSinceEpoch()));  // 随机数种子
    QString charStr = "abcdefghijklmnopqrstuvwxyz0123456789";
    QString key;
    for (int i = 0; i < len; i ++)
    {
        int idx = qrand() % (charStr.size());
        key.append(charStr[idx]);
    }
    return key;
}

QString Util::encrypt(QString content, User user, User curFriend, std::string friend_pub_e, std::string friend_pub_n)
{
    content += "[" + Util::MD5(content) + "]";
    QString AESKey = Util::getRandomKey(16);
    QString ciphertext = Util::AESEncrypt(AESKey, content); // "AES(content[MD5(content)])"

    QString header = user.getId() + "," + curFriend.getId() + ":";  // "userId,friendId:len(RSA):RSA(AES-key)"
    QString AESKeyEncryptedByRSA = Util::RSAEncrypt(friend_pub_e, friend_pub_n, AESKey);
    header += QString::number(AESKeyEncryptedByRSA.size()) + ":" + AESKeyEncryptedByRSA;

    return header + ciphertext;
}

std::pair<bool, QString> Util::decrypt(QString receiveMessage, std::string pri_d, std::string pri_n)
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

    QString AESKeyEncryptedByRSA = receiveMessage.mid(0, len);
    QString AESKey = Util::RSADecrypt(pri_d, pri_n, AESKeyEncryptedByRSA);
    QString ciphertext = receiveMessage.mid(len);
    QString message = Util::AESDecrypt(AESKey, ciphertext);

    qDebug() << message;

    QString content = message.mid(0, message.size() - 34);
    QString md5 = message.mid(content.size() + 1, 32);

    if (Util::MD5(content) == md5) return {true, content};
    else return {false, content};
}
