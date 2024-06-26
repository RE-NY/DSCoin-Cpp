#ifndef SHA256_HPP
#define SHA256_H

using namespace std; 
#include<string>
#include "Conversion.hpp"


#include <openssl/evp.h>
#include <openssl/err.h>
#include <sstream>
#include <iomanip>
#include <cassert>


class SHA256{
    public:
        SHA256(){}
        // Below function takes in a string and gives SHA256 encrypted string in hexadecimelformat(64 characs).
        static string encrypt(const string& password);  
};

string byteToHex(const unsigned char* byte_arr, size_t length);




class CRF : public SHA256 {
    public:
        int outputSize;
        //Constructor
        CRF(){}
        CRF(int size);
        string Fn(const string &s);
};




#endif // SHA256_H

#ifdef CRF_IMPLEMENTATION



string byteToHex(const unsigned char* byte_arr, size_t length) {
    stringstream ss;
    for (size_t i = 0; i < length; ++i) {
        ss << hex << setw(2) << setfill('0') << (int)byte_arr[i];
    }
    return ss.str();
}

string SHA256::encrypt(const string& password) {
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_sha256();
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, password.c_str(), password.size());
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
    EVP_MD_CTX_free(mdctx);

    return byteToHex(hash, hash_len);
}


CRF::CRF(int size){
    outputSize = size;
    assert(outputSize <= 64);
}

string CRF::Fn(const string &s){
    string shashum = encrypt(s);
    return shashum.substr(0, outputSize);
}




#endif //CRF_IMPLEMENTATION