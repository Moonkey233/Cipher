#include "cipher.h"
QString Cipher::m_rawSalt, Cipher::m_rawInput, Cipher::m_code;
const QString Cipher::Get(const QString &salt, const QString &input) {
    if (m_rawSalt == salt && m_rawInput == input) return m_code;
    QString filteredSalt = salt.toLatin1(), filteredinput = input.toLatin1(), sign, output;
    static const int subList[5] = {1, 2, 3, 5, 7};
    const int primeList[12] = {37, 31, 29, 23, 19, 17, 13, 11, 7, 5, 3, 2};
    static const QRegularExpression regexNotChar("[^A-Za-z0-9]");
    static const QRegularExpression regexNotSign("[^a-zA-Z0-9\\^()`!@#\\$%%\\^&\\*_\\-+=|\\{\\}\\[\\]:;'<>,.\\?\"%1]");
    const QString base = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    auto generateCipher = [&output, &filteredSalt, &primeList, &base]() -> const QString {
        const QString combined = (output + filteredSalt).left(12);
        int sum = 0;
        for(int i = 0; i < combined.length(); ++i) sum += combined.at(i).unicode() * primeList[i];
        return QString("%1%2%3").arg(base.at(sum%62)).arg(QString::number(sum%10)).arg(base.at((sum/62)%62));
    };
    filteredSalt = filteredSalt.remove(regexNotChar);
    if(filteredSalt.isEmpty()) return "invalid salt";
    if(filteredinput.length() < 6) return "invalid input";
    sign = filteredinput.left(5).remove(regexNotSign);
    if(sign.length() < 5) return "invalid sign";
    filteredinput = filteredinput.mid(5).remove(regexNotChar);
    if(filteredinput.isEmpty()) return "invalid input";
    for(const int &i : subList){
        if(filteredinput.length() < i) break;
        output.append(filteredinput.at(i-1));
    }
    output += filteredinput.length() == 1 ? filteredinput.repeated(3) : filteredinput.length() == 2 ? filteredinput + filteredinput.back() : filteredinput.right(3);
    output = output.toLower();
    output[0] = output[0].toUpper();
    const int linePos = output.at(0).isDigit() ? output.at(0).digitValue() : (output.at(0).toUpper().unicode() - 'A') / 10 + (output.at(0).toUpper().unicode() - 'A') % 10;
    m_rawSalt = salt, m_rawInput = input, m_code = (sign.left(2)+ output + sign.mid(3, 1) + generateCipher() + sign.mid(4, 1)).insert(linePos, sign.mid(2,1));
    return m_code;
}
