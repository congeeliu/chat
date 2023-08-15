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
