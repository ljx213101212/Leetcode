#ifndef CRYPTO_PKCS7_CONSTANT_H_
#define CRYPTO_PKCS7_CONSTANT_H_
#include "pch.h"
#include <string>
namespace crypto{

    extern const size_t kPNGHeaderSize;
    extern const long kInvalidSetFilePointer;
    extern const size_t kBufferSize;
    extern const std::string kPNGSigChunkType;
    extern const std::string kPNGIEndChunkType;
    extern const size_t kPNGChunkLengthSize;
    extern const size_t kPNGChunkTypeSize;
    extern const unsigned char kPngSignature[8];
    extern const size_t kICOHeaderSize;
    extern const size_t HASH_DIGEST_LENGTH;
    extern const uint8_t kPKCS7SignedData[9];//1.2.840.113549.1.7.2
}
#endif