#include "pkcs7_constant.h"

namespace crypto{

    const size_t kPNGHeaderSize = 8;
    const long kInvalidSetFilePointer = -1;
    const size_t kBufferSize = 0x1000;
    const uint8_t kPNGSigChunkType[4] = {'i','T','X','t'}; //"iTXt"
    const uint8_t kPNGIEndChunkType[4] = {'I','E','N','D'};//IEND
    const size_t kPNGChunkLengthSize = 4;
    const size_t kPNGChunkTypeSize = 4;
    const size_t kPNGChunkCRCSize = 4;
    const unsigned char kPngSignature[kPNGHeaderSize] = { 0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A};
    //See https://wiki.fileformat.com/image/ico
    const size_t kICOHeaderSize = 3 * sizeof(uint16_t); //6
    const size_t kICODirectoryChunkSize = 1 + 1 + 1 + 1 + 2 + 2 + 4 + 4;//16
    const size_t kICODirectoryDataSizeOffset = 1 + 1 + 1 + 1 + 2 + 2;//8
    const size_t kICODirectoryDataSizeBytes = 4;//4
    const size_t kICODirectoryDataOffsetBytes = 4;//4
    const size_t kICODirectoryDataOffsetOffset =  1 + 1 + 1 + 1 + 2 + 2 + 4; //12


	const size_t HASH_DIGEST_LENGTH = EVP_MD_size(EVP_sha256());//40
    const uint8_t kPKCS7SignedData[9] = {0x2a, 0x86, 0x48, 0x86, 0xf7,
                                           0x0d, 0x01, 0x07, 0x02};
    const std::vector<kMDOIDs> kMDOIDsVector = {
        // 1.2.840.113549.2.4
        {{0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x02, 0x04}, 8, NID_md4},
        // 1.2.840.113549.2.5
        {{0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x02, 0x05}, 8, NID_md5},
        // 1.3.14.3.2.26
        {{0x2b, 0x0e, 0x03, 0x02, 0x1a}, 5, NID_sha1},
        // 2.16.840.1.101.3.4.2.1
        {{0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01}, 9, NID_sha256},
        // 2.16.840.1.101.3.4.2.2
        {{0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x02}, 9, NID_sha384},
        // 2.16.840.1.101.3.4.2.3
        {{0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x03}, 9, NID_sha512},
        // 2.16.840.1.101.3.4.2.4
        {{0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x04}, 9, NID_sha224},
    };
}