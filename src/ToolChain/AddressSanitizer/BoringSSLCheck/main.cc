

#include "pch.h"
#include <openssl/crypto.h>
#include <openssl/bytestring.h>
#include <openssl/base.h>
#include <openssl/pkcs7.h>
#include <openssl/stack.h>
#include <openssl/mem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

// #include "openssl/base.h"
// g++ -Wall -fsanitize=address -std=c++14 -g3 -Llib/ -lcrypto -ldecrepit -lssl -I/home/jixiang/Documents/tools/boringssl/include main.cc
using namespace std;
int main(){
    cout<< CBS_ASN1_INTEGER <<endl;
    cout<< "helloworld."<<endl;
    
    std::string filePath = "keyboard_dark_sha1_signature";
    std::ifstream input(filePath, std::ios::binary);
    std::vector<char> buffer(std::istreambuf_iterator<char>(input),{});
    const unsigned char* pCertificate = reinterpret_cast<unsigned char*>(buffer.data());

    CBS cbs;
    // X509* cert = make_cert();
    //out_digest = sk_X509_new_null();

    CBS_init(&cbs, NULL, buffer.size());
    

}