#include "passwordsharing.h"
#include <QDateTime>
#include <QtMath>
#include <QVector>
#include <QDebug>


unsigned PasswordSharing::countRelativePrime()
{
    QVector<unsigned> row;
    for(unsigned i = 2; i < p; i++) row.push_back(i);

    unsigned countRelativePrime = 0;

    for(int i = 0; i < row.size(); i++)
        if(isRelativePrime(row[i], p))
            countRelativePrime++;

    return countRelativePrime;
}

unsigned PasswordSharing::gcd(unsigned a, unsigned b)
{
    while(b)
    {
        a = b;
        b = a%b;
    }

    return a;
}

bool PasswordSharing::isRelativePrime(unsigned a, unsigned b)
{
    if(gcd(a, b) == 1) return true;
    else return false;
}

unsigned PasswordSharing::calcPrimitiveRoot()
{
    unsigned eler = countRelativePrime()+1;

    for(int i = 2; i < 1000; i++)
    {
        for(int j = 2; j <= eler; j++)
        {
            if(j < eler && unsigned(qPow(i, j)) % p == 1) break;
            else if(unsigned(qPow(i, j)) % p == 1) return i;
        }
    }
}

bool PasswordSharing::isPrime(unsigned n)
{
    for(unsigned i = 2; i <= qSqrt(n); i++)
    {
        if(n % i == 0) return false;
    }

    return true;
}

unsigned PasswordSharing::modpow(unsigned base, unsigned exp, unsigned modulus)
{
    base %= modulus;
    unsigned result = 1;
    while(exp > 0)
    {
        if(exp & 1) result = (result*base) % modulus;
        base = (base*base) % modulus;
        exp >>= 1;
    }
    return result;
}

unsigned PasswordSharing::calcOpenKey()
{
    return modpow(g, closeKey, p);
}

unsigned PasswordSharing::getG()
{
    return g;
}

unsigned PasswordSharing::getP()
{
    return p;
}

unsigned PasswordSharing::getK()
{
    return k;
}

void PasswordSharing::setK(unsigned otherOpenKey)
{
    k = modpow(otherOpenKey, closeKey, p);
}

PasswordSharing::PasswordSharing(unsigned g, unsigned p, unsigned otherOpenKey) :
    g(g), p(p)
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
    closeKey = qrand() % UINT_MAX;

    setK(otherOpenKey);
}

PasswordSharing::PasswordSharing()
{
    unsigned maxVal = 1000;

    qsrand(QDateTime::currentMSecsSinceEpoch());
    closeKey = qrand() % maxVal;

    p = 2 + qrand() % maxVal;
    while(!isPrime(p)) p = 2 + qrand() % maxVal;
    g = calcPrimitiveRoot();
}
