#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QHash>

class Util
{
public:
    Util();
    static QString getConfigValue(QString group, QString key);
    static QString MD5(QString s);
    static QString strToHexStr(QString s);
    static QString hexStrToStr(QString hexStr);
    static QHash<QString, std::string> getRSAKeys();
    static QString RSAEncrypt(std::string pub_e, std::string pub_n, QString text);
    static QString RSADecrypt(std::string pri_d, std::string pri_n, QString ciphertext);
    static QString AESEncrypt(QString key, QString text);
    static QString AESDecrypt(QString key, QString hexStr);
    static QString getRandomKey(int length);
};

#endif // UTIL_H
