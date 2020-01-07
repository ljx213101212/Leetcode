#include "pkcs7_constant.h"

namespace crypto{

    const size_t kPNGHeaderSize = 8;
    const long kInvalidSetFilePointer = -1;
    const size_t kBufferSize = 0x1000;
    const std::string kPNGSigChunkType = "iTXt";
    const std::string kPNGIEndChunkType ="IEND";
    const size_t kPNGChunkLengthSize = 4;
    const size_t kPNGChunkTypeSize = 4;
    const unsigned char kPngSignature[kPNGHeaderSize] = { 0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A};
    const size_t kICOHeaderSize = 3 * sizeof(uint16_t); //6
    const size_t HASH_DIGEST_LENGTH = EVP_MD_size(EVP_sha256());//40
    const uint8_t kPKCS7SignedData[9] = {0x2a, 0x86, 0x48, 0x86, 0xf7,
                                           0x0d, 0x01, 0x07, 0x02};
}