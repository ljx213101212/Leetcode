

#include "pch.h"
#include "pkcs7_verifier.h"

// #include "openssl/base.h"
// g++ -Wall -fsanitize=address -std=c++14 -g3 -o mymain pkcs7_constant.cc pkcs7_verifier.cc main.cc -L/home/jixiang/Documents/code/Leetcode/src/ToolChain/AddressSanitizer/BoringSSLCheck/lib -lcrypto -I/home/jixiang/Documents/tools/boringssl/include -I/home/jixiang/Documents/tools/boringssl/crypto
// export LD_LIBRARY_PATH=/home/jixiang/Documents/code/Leetcode/src/ToolChain/AddressSanitizer/BoringSSLCheck/lib:$LD_LIBRARY_PATH
using namespace std;


int main(){

    std::string filePath = "/home/jixiang/Documents/code/Leetcode/src/ToolChain/AddressSanitizer/BoringSSLCheck/keyboard_dark_sha256_signature";
    std::string icoPath = "";
    std::ifstream input(filePath, std::ios::binary);
    std::vector<char> buffer(std::istreambuf_iterator<char>(input),{});
    const unsigned char* pCertificate = reinterpret_cast<unsigned char*>(buffer.data());

    do {
        crypto::PKCS7Verifier pkcs7_verifier(pCertificate, buffer.size());
        std::vector<uint8_t> public_key_info_vector;
        std::vector<uint8_t> signer_info;
        std::vector<uint8_t> encrypted_digest;

        // File Content Verification
        //pkcs7_verifier.PKCS7FileContentDigestVerification(&cbs_input, )

        // Signature Verification
        CBS cbs_input;
        cbs_input = pkcs7_verifier.GetSignedData();
        CBS cbs_algo;
        pkcs7_verifier.PKCS7GetDigestAlgorithm(&cbs_input, &cbs_algo);
        const EVP_MD* evp_algo = crypto::PKCS7Verifier::cbs_to_md(&cbs_algo);        
        cbs_input = pkcs7_verifier.GetSignedData();
        bool test_res = pkcs7_verifier.PKCS7MessageDigestValidation(&cbs_input, evp_algo);
        cbs_input = pkcs7_verifier.GetSignedData();
        pkcs7_verifier.PKCS7GetPublicKeyInfo(&cbs_input, public_key_info_vector);
        cbs_input = pkcs7_verifier.GetSignedData();
        pkcs7_verifier.PKCS7GetSignerInfo(&cbs_input, signer_info);
        cbs_input = pkcs7_verifier.GetSignedData();
        pkcs7_verifier.PKCS7GetEncryptedDigest(&cbs_input, encrypted_digest);
    }while(0);
    cout<< "OK."<<endl;
}