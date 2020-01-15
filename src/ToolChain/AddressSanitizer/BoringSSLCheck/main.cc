

#include "pch.h"
#include "pkcs7_verifier.h"

// #include "openssl/base.h"
// g++ -Wall -fsanitize=address -std=c++14 -g3 -o mymain pkcs7_constant.cc pkcs7_verifier.cc main.cc -L/home/jixiang/Documents/code/Leetcode/src/ToolChain/AddressSanitizer/BoringSSLCheck/lib -lcrypto -I/home/jixiang/Documents/tools/boringssl/include -I/home/jixiang/Documents/tools/boringssl/crypto
// export LD_LIBRARY_PATH=/home/jixiang/Documents/code/Leetcode/src/ToolChain/AddressSanitizer/BoringSSLCheck/lib:$LD_LIBRARY_PATH
// generate ASAN report:
// ASAN_OPTIONS=log_path=mylog.log ./mymain
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
        base::FilePath::StringType st( FILE_PATH_LITERAL("C:\\Users\\jixiang.li\\AppData\\Local\\Chromium\\User Data\\Apps\\helloworld_folder\\png\\ball.png"));
        base::FilePath::StringType st_ico(FILE_PATH_LITERAL("C:\\Users\\jixiang.li\\AppData\\Local\\Chromium\\User Data\\Apps\\helloworld_folder\\ico\\keyboard_dark_sha256.ico"));
        base::FilePath png_file_path(st);
        base::FilePath ico_file_path(st_ico);

        //1. Get PNG file content digest.
        //1.1 Init pkcs7_png instance.
        crypto::PKCS7Png pkcs7_png(png_file_path);
        size_t png_digest_size = 0;
        std::unique_ptr<uint8_t[]> png_file_digest;
        //1.2 get png digest size.
        pcks7_png.getSignedFileDigest(png_digest_size, NULL);
        png_file_digest.reset(new uint8_t[png_digest_size]);
        //1.3 get png digest data.
        pkcs7_png.getSignedFileDigest(png_digest_size, file_digest.get());

        //2. Get ICO file content digest.
        //2.1 Init pkcs7_ico instance.
        crypto::PKCS7Ico pkcs7_ico(ico_file_path);
        size_t ico_digest_size = 0;
        pkcs7_ico.getSignedFileDigest(ico_digest_size, NULL);
        std::vector<uint8_t> ico_file_digest(ico_digest_size, {});
        pkcs7_ico.getSignedFileDigest(ico_digest_size, ico_file_digest.data());

        //3, Begin Content Digest Verification & Signature Verification.
        CBS cbs_input;
        cbs_input = pkcs7_verifier.GetSignedData();
        //Get digest algorithm
        CBS cbs_algo;
        pkcs7_verifier.PKCS7GetDigestAlgorithm(&cbs_input, &cbs_algo);
        //Content Digest Verification
        const EVP_MD* evp_algo = crypto::PKCS7Verifier::cbs_to_md(&cbs_algo);
        cbs_input = pkcs7_verifier.GetSignedData();
        std::vector<uint8_t> png_file_content_digest(EVP_MD_size(evp_algo));
        std::vector<uint8_t> ico_file_content_digest(EVP_MD_size(evp_algo));
        unsigned int file_content_digest_size = 0;
        pkcs7_png.getFileContentDigest(evp_algo, file_content_digest_size, png_file_content_digest.data());
        pkcs7_ico.getFileContentDigest(evp_algo, file_content_digest_size, ico_file_content_digest.data());
        CBS cbs_ico_file_content_digest;
        // CBS cbs_png_file_content_digest;
        CBS_init(&cbs_ico_file_content_digest, ico_file_content_digest.data(),ico_file_content_digest.size());	
        // CBS_init(&cbs_png_file_content_digest, png_file_content_digest.data(),png_file_content_digest.size());	
        pkcs7_verifier.PKCS7FileContentDigestVerification(&cbs_input, &cbs_ico_file_content_digest);
        // pkcs7_verifier.PKCS7FileContentDigestVerification(&cbs_input, &cbs_png_file_content_digest);

        // Signature Verification        
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