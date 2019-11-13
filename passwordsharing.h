#ifndef PASSWORDSHARING_H
#define PASSWORDSHARING_H


class PasswordSharing
{
private:
    unsigned g;
    unsigned p;
    unsigned k;
    unsigned closeKey;

    unsigned calcPrimitiveRoot();
    unsigned countRelativePrime();
    unsigned gcd(unsigned a, unsigned b);
    bool isRelativePrime(unsigned a, unsigned b);

    bool isPrime(unsigned n);
    unsigned modpow(unsigned base, unsigned exp, unsigned modulus);
public:
    unsigned calcOpenKey();
    unsigned getG();
    unsigned getP();

    void setK(unsigned otherOpenKey);
    unsigned getK();

    PasswordSharing(unsigned g, unsigned p, unsigned otherOpenKey);
    PasswordSharing();
};

#endif // PASSWORDSHARING_H
