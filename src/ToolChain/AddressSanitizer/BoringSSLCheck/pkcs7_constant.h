#ifndef CRYPTO_PKCS7_CONSTANT_H_
#define CRYPTO_PKCS7_CONSTANT_H_
#include <string>
#include <vector>
#include <fstream>
#include "third_party/boringssl/src/include/openssl/base.h"
#include "third_party/boringssl/src/include/openssl/bytestring.h"
#include "third_party/boringssl/src/include/openssl/digest.h"
#include "third_party/boringssl/src/include/openssl/evp.h"
#include "third_party/boringssl/src/include/openssl/rsa.h"
#include "third_party/boringssl/src/include/openssl/pkcs7.h"
#include "third_party/boringssl/src/include/openssl/mem.h"

namespace crypto{

    extern const size_t kPNGHeaderSize;
    extern const long kInvalidSetFilePointer;
    extern const size_t kBufferSize;
    extern const uint8_t kPNGSigChunkType[4];
    extern const uint8_t kPNGIEndChunkType[4];
    extern const size_t kPNGChunkLengthSize;
    extern const size_t kPNGChunkTypeSize;
    extern const size_t kPNGChunkCRCSize;
    extern const unsigned char kPngSignature[8];
    extern const size_t kICOHeaderSize;
    extern const size_t kICODirectoryChunkSize;
    extern const size_t kICODirectoryDataSizeOffset;
    extern const size_t kICODirectoryDataSizeBytes;
    extern const size_t kICODirectoryDataOffsetBytes;
    extern const size_t kICODirectoryDataOffsetOffset;
	extern const size_t HASH_DIGEST_LENGTH;
    extern const uint8_t kPKCS7SignedData[9];//1.2.840.113549.1.7.2
    typedef struct kMDOIDs{
        uint8_t oid[9];
        uint8_t oid_len;
        int nid;
    }kMDOIDs;

    extern const std::vector<kMDOIDs> kMDOIDsVector;
}
#endif