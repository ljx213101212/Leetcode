#include "pkcs7_png.h"

namespace crypto{

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
            const char* tag = &data_read[4];
        
            if (std::memcmp(tag, kPNGSigChunkType, kPNGChunkTypeSize) != 0)
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

    //private
    //See https://www.w3.org/TR/2003/REC-PNG-20031110/#5Chunk-layout
    bool PKCS7Png::hashFileContent(base::File* file, EVP_MD_CTX* ctx){
        if (NULL == file ||
            NULL == ctx) { return false; }
        
        char data_read[kBufferSize];
        //Hash header
        if (file->ReadAtCurrentPos(data_read, kPNGHeaderSize) <= 0) { return false; }
        if (1 != EVP_DigestUpdate(ctx, data_read, kPNGHeaderSize)){ return false; }
        //Hash body

        size_t bytes_read;
        //Read until file reach the end.
        while((bytes_read = file->ReadAtCurrentPos(data_read, kPNGHeaderSize)) > 0){
            
            //when the file is corrupted.
            if (bytes_read != kPNGHeaderSize) { return false;}
            //See https://www.w3.org/TR/2003/REC-PNG-20031110/#7Integers-and-byte-order
            //(network byte order)big-endian 
            const unsigned int tag_size = data_read[3] | data_read[2] << 8 | data_read[1] << 16 | data_read [0] << 24;
            const char* tag = (&data_read[4]);

            //If find the signature chunk, skip this chunk, do not hash this part.
            if (0 == memcmp(tag, kPNGSigChunkType, kPNGChunkTypeSize)){
                if (file->Seek(base::File::FROM_CURRENT, tag_size + kPNGChunkCRCSize) == kInvalidSetFilePointer){ return false; }
                continue;
            }
            //hash current chunk header (LENGTH chunk + TYPE chunk)
            if (1 != EVP_DigestUpdate(ctx, data_read, kPNGHeaderSize)){ return false; }
            for (unsigned int chunk_id = 0; chunk_id / kBufferSize; chunk_id++){
                if (file->ReadAtCurrentPos(data_read, kBufferSize) <= 0) { return false; }
                if (1 != EVP_DigestUpdate(ctx, data_read, kBufferSize)){ return false; }
            }
            //hash reminder data(below kBufferSize part) + CRC CHUNK 
            size_t remainderSize = (tag_size % kBufferSize) + kPNGChunkCRCSize;
            if (file->ReadAtCurrentPos(data_read, remainderSize) <= 0) { return false; }
            if (1 != EVP_DigestUpdate(ctx, data_read, remainderSize)){ return false; }
        }
        return true;
    }

    bool PKCS7Png::getFileContentDigest(const EVP_MD* in_digest_algorithm, unsigned int &out_file_content_digest_size, uint8_t *out_file_content_digest){
        
         //Param check
        if (out_file_content_digest == NULL) { return false; }
        bssl::ScopedEVP_MD_CTX mdctx;
        if (1 != EVP_DigestInit_ex(mdctx.get(), in_digest_algorithm, NULL)){ return false; }
        base::File file = getFile();
        if (file.Seek(base::File::FROM_BEGIN, kPNGHeaderSize) == kInvalidSetFilePointer){
            return false;
        }
        hashFileContent(&file, mdctx.get());
        if (1 != EVP_DigestFinal_ex(mdctx.get(), out_file_content_digest, &out_file_content_digest_size)){ return false; }
        return true;
    }
}