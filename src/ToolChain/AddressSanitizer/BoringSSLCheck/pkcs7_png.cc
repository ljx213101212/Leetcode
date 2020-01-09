#include "pkcs7_png.h"
#include "pkcs7_constant.h"

namespace crypto{

    // static const size_t kPNGHeaderSize = 8;
    // static const long kInvalidSetFilePointer = -1;
    // static const size_t kBufferSize = 0x1000;
    // static const std::string kPNGSigChunkType("iTXt");
    // static const std::string kPNGIEndChunkType("IEND");
    // static const size_t kPNGChunkLengthSize = 4;
    // static const size_t kPNGChunkTypeSize = 4;

    PKCS7Png::PKCS7Png(base::FilePath file_path){
        file_path_ = file_path;
    }
    PKCS7Png::~PKCS7Png(){
    }

    base::File PKCS7Png::getFile(){
        return base::File(file_path_, base::File::FLAG_OPEN | base::File::FLAG_READ);
    }

    int64_t PKCS7Png::getFileSize(){
        base::File::Info info;
        base::File file = getFile();
        file.GetInfo(&info);
        return info.size;
    }

    //Call this method to get file_digest_size first (pass file_digest as NULL)
    //Then call this method again to get the file_digest itself.
    bool PKCS7Png::getSignedFileDigest(size_t &file_digest_size, uint8_t *file_digest) {
        
        base::File file = getFile();
        if (file.Seek(base::File::FROM_BEGIN, kPNGHeaderSize) == kInvalidSetFilePointer){
            return false;
        }
        size_t total_read = 0; 
        char data_read[kBufferSize];
        for (;;)
        {
            if (file.ReadAtCurrentPos(&data_read[0], kPNGHeaderSize) <= 0) { return false;}
            //See http://www.libpng.org/pub/png/book/chapter08.html  - big-endian
            //See https://www.w3.org/TR/2003/REC-PNG-20031110/#7Integers-and-byte-order - the most significant byte comes first
            const unsigned int size = data_read[3] | data_read[2] << 8 | data_read[1] << 16 | data_read[0] << 24;
            const unsigned char* tag = ((const unsigned char*)&data_read[4]);
        
            if (std::memcmp(tag, kPNGSigChunkType.c_str(), kPNGChunkTypeSize) != 0)
            {
                //Seek through current chunk's data chunk and the next chunk's length chunk
                if (file.Seek(base::File::FROM_CURRENT, size + kPNGChunkLengthSize) == kInvalidSetFilePointer){
                    return false;
                }
                continue;
            }
            // Win32 is asking how big of a buffer it needs. Set the size and exit.
            if (file_digest == NULL)
            {
                file_digest_size = size;
                return true;
            }
            // It supplied a buffer, but it wasn't big enough.
            else if (file_digest_size < size)
            {
                return false;
            }
            for (size_t i = 0; i < size / kBufferSize; i++)
            {
                if (file.ReadAtCurrentPos(&data_read[0], kBufferSize) <= 0) { return false; }
                std::memcpy(file_digest + total_read, &data_read[0], kBufferSize);
                total_read += kBufferSize;
            }
            size_t remainder = size % kBufferSize;
            if (remainder > 0)
            {
                if (file.ReadAtCurrentPos(&data_read[0], remainder) <= 0) { return false; }
                std::memcpy(file_digest + total_read, &data_read[0], remainder);
                total_read += remainder;
            }
            file_digest_size = total_read;
            return true;
        }
        return false;
    }
}