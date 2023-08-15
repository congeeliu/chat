#include "rsa.h"
#include "stdlib.h" // for rand()

#include <QString>
#include <QDebug>

#define RSA_DBG 0

using namespace std;

mpz_class RSA::randbits(int bits) //base = 2
{
    gmp_randclass a(gmp_randinit_default); //gmp_randinit  gmp_randinit_default
    unsigned long r = static_cast<unsigned long>(rand());
    a.seed(r);
    mpz_class l(bits);
    return a.get_z_bits(l);
}

inline mpz_class RSA::randprime(int bits)
{
    mpz_class a = randbits(bits);
    mpz_class ret;
    mpz_nextprime(ret.get_mpz_t(),a.get_mpz_t());
    return ret;
}

void RSA::createKey(mpz_class &n, mpz_class &e, mpz_class &d, const int nbits, int ebits=16)
{
    if(nbits/2<=ebits) {
        ebits = nbits/2;
    }
    mpz_class p = randprime(nbits/2);
    mpz_class q = randprime(nbits/2);
    n = q*p;
    mpz_class fn = (p-1)*(q-1);
    mpz_class gcd;
    do{
        e = randprime(ebits);
        mpz_gcd(gcd.get_mpz_t(),e.get_mpz_t(),fn.get_mpz_t());
    }while(gcd!=1);
    mpz_gcdext(p.get_mpz_t(),d.get_mpz_t(),q.get_mpz_t(),e.get_mpz_t(),fn.get_mpz_t());
}

inline mpz_class RSA::encrypt(const mpz_class &m,const mpz_class &e,const mpz_class &n)
{
    mpz_class ret;
    mpz_powm(ret.get_mpz_t(),m.get_mpz_t(),e.get_mpz_t(),n.get_mpz_t());
    return ret;
}

inline mpz_class RSA::decrypt(const mpz_class &c,const mpz_class &d,const mpz_class &n)
{
    return encrypt(c, d, n);
}

int RSA::test()
{
    string pub_e, pub_n, pri_d, pri_n;
/*
    generateKey(512, 32);

    getKey(RSA::PublicKey, pub_e, pub_n);
    cout<<"Pub key:"<<pub_e.c_str()<<", "<<pub_n.c_str()<<endl;

    getKey(RSA::PrivateKey, pri_d, pri_n);
    cout<<"Pri key:"<<pri_d.c_str()<<", "<<pri_n.c_str()<<endl;
*/
    //n=512bit e=16bit
    pub_e = "e652f9e9";
    pub_n = "bc92db32524ee6f04de2a12f0164668f1fef73959c17bcfe1417f2483e4f85acb879b15862182f510bd6c9d7ba639d105be09df5fc2a857623a977a2ebdbf";

    pri_d = "-24f459b7f2794eae9cae6ca735afca8a9958b7cd3c522c4cca6363ed3c3897fa6ee6d946857951671861ed0670260b1c3c7b6c44c1a2f5eceefabd792dc27";
    pri_n = "bc92db32524ee6f04de2a12f0164668f1fef73959c17bcfe1417f2483e4f85acb879b15862182f510bd6c9d7ba639d105be09df5fc2a857623a977a2ebdbf";

    setKey(RSA::PublicKey,  pub_e, pub_n);
    setKey(RSA::PrivateKey, pri_d, pri_n);

    string text = "1234567890";
    string enc = Encrypt(text, 10);

    qDebug() << QString::fromStdString(enc);

    string dec = Decrypt(enc.c_str(), 10);

    qDebug() << QString::fromStdString(dec);

    return 0;
}



RSA::RSA()
{

}
RSA::~RSA()
{

}
void RSA::generateKey(int nbits, int ebits=16)
{
    createKey(key_n, key_e, key_d, nbits, ebits);
}


void RSA::getKey(KeyType type, string &ed, string &sn)
{
    if(type == PublicKey)
    {
#if RSA_DBG
        cout<<"get public  key:("<<key_e.get_str(16)<<", "<<key_n.get_str(16)<<")"<<endl;
#endif
        ed = key_e.get_str(16);
        sn = key_n.get_str(16);
    }
    else
    {
#if RSA_DBG
        cout<<"get private key:("<<key_d.get_str(16)<<", "<<key_n.get_str(16)<<")"<<endl;
#endif
        ed = key_d.get_str(16);
        sn = key_n.get_str(16);
    }
}
void RSA::setKey(KeyType type, string &ed, string &sn)
{
    if(type == PublicKey)
    {

        key_e.set_str(ed.c_str(), 16);
        key_n.set_str(sn.c_str(), 16);
#if RSA_DBG
        cout<<"set public  key:("<<key_e.get_str(16)<<", "<<key_n.get_str(16)<<")"<<endl;
#endif
    }
    else
    {
        key_d.set_str(ed.c_str(), 16);
        key_n.set_str(sn.c_str(), 16);
#if RSA_DBG
        cout<<"set private key:("<<key_d.get_str(16)<<", "<<key_n.get_str(16)<<")"<<endl;
#endif
    }
}

string RSA::Encrypt(string in, int base)
{

    mpz_class m;
    m.set_str(in.c_str(), base);


    mpz_class c;
    c = encrypt(m, key_e, key_n);
#if RSA_DBG
    cout<<"after encrypt:"<<c.get_str(base)<<endl;
#endif
    return c.get_str(base);
}

string RSA::Decrypt(string in, int base)
{

    mpz_class m;
    m.set_str(in.c_str(), base);

    mpz_class c;
    c = decrypt(m, key_d, key_n);
#if RSA_DBG
    cout<<"after decrypt:"<<c.get_str(base)<<endl;
#endif
    return c.get_str(base);
}



