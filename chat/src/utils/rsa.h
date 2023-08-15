#ifndef RSA_H
#define RSA_H
#include <iostream>
#include <gmpxx.h>
#include <cstdlib>

using namespace std;

class RSA
{

public:
    RSA();
    ~RSA();
    enum KeyType{PublicKey  = 0,PrivateKey = 1};

    string Encrypt(string in, int base);
    string Decrypt(string in, int base);

    void generateKey(int nbits, int ebits);

    void getKey(KeyType type, string &ed, string &sn);

    void setKey(KeyType type, string &ed, string &sn);

    int test();

private:
    mpz_class randbits(int bits);

    inline mpz_class randprime(int bits);

    void createKey(mpz_class &n, mpz_class &e, mpz_class &d,const int nbits,int ebits);

    inline mpz_class encrypt(const mpz_class &m,const mpz_class &e,const mpz_class &n);

    inline mpz_class decrypt(const mpz_class &c,const mpz_class &d,const mpz_class &n);

    mpz_class key_n, key_e, key_d;
};

#endif // RSA_H

