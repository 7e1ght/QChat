#include "crypt.h"

QString Crypt::encrypt(QString data, unsigned int key)
{
    for(unsigned i = 0; i < data.size(); i++)
    {
        data.replace(i, 1, QChar(data[i].unicode() + key));
    }

    return data;
}

QString Crypt::decrypt(QString data, unsigned int key)
{
    for(unsigned i = 0; i < data.size(); i++)
    {
        data.replace(i, 1, QChar(data[i].unicode() - key));
    }

    return data;
}

Crypt::Crypt()
{

}
