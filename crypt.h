#ifndef CRYPT_H
#define CRYPT_H

#include <QString>

class Crypt
{
private:
    Crypt();
public:
    static QString encrypt(QString data, unsigned key);
    static QString decrypt(QString data, unsigned key);
};

#endif // CRYPT_H
