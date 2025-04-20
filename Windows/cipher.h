#ifndef CIPHER_H
#define CIPHER_H
#include <QString>
#include <QRegularExpression>
class Cipher{
public:
    static const QString Get(const QString &salt, const QString &input);
private:
    static QString m_rawSalt, m_rawInput, m_code;
};
#endif // CIPHER_H
