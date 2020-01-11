#include "pkcs7_ico.h"
#include "pkcs7_constant.h"
#if defined(OS_WIN)
#include <winsock.h>
#elif defined(OS_POSIX) || defined(OS_FUCHSIA)
#include <arpa/inet.h>
#else
#error Unsupported platform
#endif


namespace crypto{

    PKCS7Ico::PKCS7Ico(base::FilePath file_path){
        file_path_ = file_path;
        GetIcoFileInfo(&file_info_);
    }
    PKCS7Ico::~PKCS7Ico() = default;
    
    size_t PKCS7Ico::GetFileCurrentPos(base::File& file){
        return file.Seek(base::File::FROM_CURRENT, 0);
    }

    base::File PKCS7Ico::getFile(){
        return base::File(file_path_, base::File::FLAG_OPEN | base::File::FLAG_READ);
    }

    //private
    bool PKCS7Ico::UpdateSignaturePosition(unsigned char* png_chunk_data, size_t png_chunk_size, size_t png_chunk_offset, IcoFileInfo* info){
        
        if (NULL == png_chunk_data || png_chunk_size == 0)
        unsigned char data_read[kPNGHeaderSize];
        size_t png_chunk_ptr = 0;
        png_chunk_ptr += kPNGHeaderSize;
        while (png_chunk_ptr < png_chunk_size){
            memcpy_s(data_read, kPNGHeaderSize, png_chunk_data + png_chunk_ptr, kPNGHeaderSize);
            uint32_t png_size = ntohl(*(uint32_t*)data_read);
            const unsigned char* tag = ((const unsigned char*)&buffer[4]);
            if (0 == memcmp(tag, kPNGSigChunkType, PNG_TAG_SIZE)){
                info->sig_chunk_offset = png_chunk_offset + png_chunk_ptr;
                info->sig_chunk_size = kPNGHeaderSize + png_size + kPNGChunkCRCSize;
                return true;
            }
            png_chunk_ptr += kPNGHeaderSize + size + kPNGChunkCRCSize;
        }
        return NULL;
    }

    int PKCS7Ico::ReadIcoImageNumber(){

        uint16_t input;
        base::File file = getFile();
        if (file.ReadAtCurrentPos((char*)&input, sizeof(uint16_t)) <= 0) { return -1;}
        // Was it 'reserved' ?   (ie 0)
        if (input != 0)
            return -1;
        // Read the type WORD
        if (file.ReadAtCurrentPos((char*)&input, sizeof(uint16_t)) <= 0) { return -1;}
        // Was it type 1?
        if (input != 1)
            return -1;
        // Get the count of images
        if (file.ReadAtCurrentPos((char*)&input, sizeof(uint16_t)) <= 0) { return -1;}
        // Return the count
        return (int)input;
    }

    bool PKCS7Ico::CheckPngSignature(unsigned char* chunkData) {
        unsigned char bufferSIG[kPNGHeaderSize];
        memcpy_s(&bufferSIG, kPNGHeaderSize, chunkData, kPNGHeaderSize);
        return (0 == memcmp(bufferSIG, kPngSignature, kPNGHeaderSize));
    }

    //private
    bool PKCS7Ico::GetIcoFileInfo(IcoFileInfo *info){

        std::unique_ptr<ICONRESOURCE>lpIR_ptr = std::make_unique<ICONRESOURCE>();
        std::unique_ptr<ICONDIRENTRY[]>lpIDE_ptr = std::make_unique<ICONDIRENTRY[]>(1);
        unsigned int i = 0;
        if (info == nullptr) { return false; }
        base::File file = getFile();
        info->file_start_position = GetFileCurrentPos(file);

        LPICONRESOURCE lpIR = lpIR_ptr.get();
        LPICONDIRENTRY lpIDE = lpIDE_ptr.get();
        // Allocate memory for the resource structure
        // Read in the header
        if ((lpIR->nNumImages = ReadIcoImageNumber()) == (unsigned int)-1) { return false; }
        // Adjust file pointer
        if (file.Seek(base::File::FROM_CURRENT, kICOHeaderSize) == kInvalidSetFilePointer){
            return false;
        }
        // Adjust the size of the struct to account for the images
        // lpIR_ptr.reset(new ICONRESOURCE[sizeof(ICONRESOURCE) + ((lpIR->nNumImages - 1) * sizeof(ICONIMAGE))]);
        // lpIR = lpIR_ptr.get();
        // std::realloc(lpIR, sizeof(ICONRESOURCE) + ((lpIR->nNumImages - 1) * sizeof(ICONIMAGE)));
        lpIR->IconImages = std::make_unique<ICONIMAGE[]>(sizeof(ICONRESOURCE) + ((lpIR->nNumImages - 1) * sizeof(ICONIMAGE)));
        // Allocate enough memory for the icon directory entries
        // lpIDE_ptr.reset(new ICONDIRENTRY[lpIR->nNumImages * sizeof(ICONDIRENTRY)]);
        // lpIDE = lpIDE_ptr.get();
        // std::realloc(lpIDE, lpIR->nNumImages * sizeof(ICONDIRENTRY));
        lpIDE_ptr.reset(new ICONDIRENTRY[lpIR->nNumImages * sizeof(ICONDIRENTRY)]);
        lpIDE = lpIDE_ptr.get();
        // Read in the icon directory entries
        if (file.ReadAtCurrentPos((char*)lpIDE, lpIR->nNumImages * sizeof(ICONDIRENTRY)) <= 0) { return -1;}

        // Loop through and read in each image
        for (i = 0; i < lpIR->nNumImages; i++)
        {
            
            lpIR->IconImages.get()[i].dwNumBytes = lpIDE[i].dwBytesInRes;
            // Seek to beginning of this image
             if (file.Seek(base::File::FROM_BEGIN, lpIDE[i].dwImageOffset) == kInvalidSetFilePointer){
                    return false;
            }
            // Allocate memory for the resource
            lpIR->IconImages.get()[i].lpBits = std::make_unique<unsigned char far[]>(lpIDE[i].dwBytesInRes);
            // Read it in
            if (file.ReadAtCurrentPos((char*)lpIR->IconImages.get()[i].lpBits.get(), lpIDE[i].dwBytesInRes) <= 0) { return -1;}

            //check PNG SIG
            if (CheckPngSignature(lpIR->IconImages.get()[i].lpBits.get())) {
                info->png_end_position = GetFileCurrentPos(file);
                info->png_start_position = info->png_end_position - lpIDE[i].dwBytesInRes;
                info->nth_image_is_png = i + 1;
                info->num_of_ico = lpIR->nNumImages;
                info->png_chunk_offset = lpIDE[i].dwImageOffset;
                info->png_chunk_size = lpIDE[i].dwBytesInRes;
                UpdateSignaturePosition(lpIR->IconImages[i].lpBits, info->png_chunk_size, info->png_chunk_offset, info);
            }
    
            // Set the internal pointers appropriately
        }
        info->file_end_position = GetFileCurrentPos(file);
        return true;
    }

    bool PKCS7Ico::getSignedFileDigest(size_t &file_digest_size, uint8_t *file_digest){

        if (file_info_ == NULL) { return false;}
        IcoFileInfo info = file_info_;
        size_t png_size = info.png_end_position - info.png_start_position;
        base::File file = getFile();
        if (file.Seek(base::File::FROM_BEGIN, info.png_start_position + kPNGHeaderSize) == kInvalidSetFilePointer){
            return false;
        }
        size_t total_read = 0;
	    char data_read[kBufferSize];
		for (;total_read < png_size;)
		{
		    if (file.ReadAtCurrentPos(&data_read[0], kPNGHeaderSize) <= 0) { return  false;}

			const unsigned int size = data_read[3] | data_read[2] << 8 | data_read[1] << 16 | data_read[0] << 24;
			const unsigned char* tag = ((const unsigned char*)&data_read[4]);
			if (std::memcmp(tag, kPNGSigChunkType.c_str(), kPNGChunkTypeSize) != 0)
			{
                if (file.Seek(base::File::FROM_CURRENT, size + kPNGChunkTypeSize) == kInvalidSetFilePointer){
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

    //Note: See https://wiki.fileformat.com/image/ico/#header
    //All values in ICO/CUR files are represented in little-endian byte order.
    //Because signed ico file length changed, recover the original ico header 
    bool PKCS7Ico::HashFileHeader(base::File* file, EVP_MD_CTX* ctx) {
        
        //Param check
        if (file == NULL || ctx == NULL) { return false; }
     
        //Get the ICO file basic info.
        if (file_info_ == NULL) { return false;}
        IcoFileInfo info = file_info_;
        //Start to recover the original ico header.
        //See https://wiki.fileformat.com/image/ico/
        //Note:
        //1.In ico header chunk,  all bmp chunk header info "offset" after png need to be updated back to the original data.
        //2.In ico header chunk,  one and the only one png chunk "size" value need to be updated bakc to the original data.
        size_t png_size_offset = kICOHeaderSize + (info.nth_image_is_png - 1) * kICODirectoryChunkSize + kICODirectoryDataSizeOffset;
        size_t ico_header_directory_size = kICOHeaderSize + info.num_of_ico * kICODirectoryChunkSize;
        //ico_header_directory_size usually is very short around hundreds of bytes.
        std::vector<char> data_read(ico_header_directory_size);
        //Init the value need to be hashed.
        std::vector<char> header_directory_buffer(ico_header_directory_size);
        //start from the first byte of icon file.
        size_t ico_file_ptr = 0;
        size_t png_size_data = 0;
        size_t rest_num_of_bmp = 0;

        //Make sure the file ptr is from the beginning
        if (file.Seek(base::File::FROM_BEGIN, 0) == kInvalidSetFilePointer){
            return false;
        }
        //Read ico header and directory chunk.
        if (file.ReadAtCurrentPos(header_directory_buffer.data(), ico_header_directory_size) <= 0) { return false; }

        /**
         * Update png chunk size value and pass data from ico first start byte to end of png in directory byte. 
         */ 
        //Update ptr to the first byte of directory of the one and only png size.
        ico_file_ptr = png_size_offset;
        //Read 4 bytes of png size (little endian)
        memcpy_s(data_read.data(), kICODirectoryDataSizeBytes, header_directory_buffer.data() + ico_file_ptr, kICODirectoryDataSizeBytes);
        uint32_t png_size = ntohl(*(uint32_t*)data_read.data());
        //revert back to origin data.
        png_size -= info.sig_chunk_size;
        memcpy_s(data_read.data(), kICODirectoryDataSizeBytes, &png_size, kICODirectoryDataSizeBytes);
        //write in head directory buffer.
        memcpy_s(header_directory_buffer.data() + ico_file_ptr, kICODirectoryDataSizeBytes, data_read.data(), kICODirectoryDataSizeBytes);
        //move file ptr to pass png header of offset 8 (8) and offset 12 (C).
        ico_file_ptr += kICODirectoryDataSizeBytes + kICODirectoryDataOffsetBytes;

        /**
         * Update all bmp chunks follow png chunk(one and only) in ico directory chunk.
         */ 
        rest_num_of_bmp = info.numOfIco - info.nthImageIsPng;
        for (int i = 0; i < rest_num_of_bmp; i++){

            ico_file_ptr += kICODirectoryDataOffsetOffset;
            unsigned char offset_buffer[kICODirectoryDataOffsetBytes];
            memcpy_s(offset_buffer, kICODirectoryDataOffsetBytes, header_directory_buffer.data() + ico_file_ptr,kICODirectoryDataOffsetBytes);

            uint32_t offset = ntohl(*(uint32_t*)offset_buffer);
            offset -= info.sig_chunk_size;
            //update offset_buffer
            memcpy_s(offset_buffer, kICODirectoryDataOffsetBytes, &offset, kICODirectoryDataOffsetBytes);
            //write in head directory buffer
            memcpy_s(header_directory_buffer.data() + ico_file_ptr, kICODirectoryDataOffsetBytes, offset_buffer, kICODirectoryDataOffsetBytes);
            //move file ptr to pass offset 12 (C)
            ico_file_ptr += kICODirectoryDataOffsetBytes;
        }

        /**
         * Hash the "Transformed back to original" file  (only header and directory part)
         */ 
        if (1 != EVP_DigestUpdate(ctx, header_directory_buffer.data(), ico_header_directory_size)){ return false; }
         
    }

    //Continuing hash the file but skip the signature part (icons content)
    //This method need to be called after HashFileHeader. 
    bool PKCS7Ico::HashFileBody(base::File* file, EVP_MD_CTX* ctx){

        //Param check
        if (file == NULL || ctx == NULL) { return false; }
        //Get the ICO file basic info.
        if (file_info_ == NULL) { return false;}
        IcoFileInfo info = file_info_;
        size_t ico_header_directory_size = kICOHeaderSize + info.num_of_ico * kICODirectoryChunkSize;
        size_t total_body_size =  info.file_end_position - ico_header_directory_size;
        size_t before_signature_part_size = info.sig_chunk_offset - ico_header_directory_size;
        size_t before_signature_part_offset = ico_header_directory_size;
        size_t after_singature_part_offset = info.sig_chunk_offset + info.sig_chunk_size; 
        size_t after_signature_part_size = info.file_end_position - after_singature_part_offset;
        //1. Hash before signature part chunk
        HashFixedChunk(file, ctx, before_signature_part_size, before_signature_part_offset);
        //2. Hash after signature part chunk
        HashFixedChunk(file, ctx, after_signature_part_size, after_signature_part_offset);
        return true;
    }


    bool PKCS7Ico::HashFixedChunk(base::File* file, EVP_MD_CTX* ctx, size_t chunk_size, size_t chunk_offset){
        
        //Param check
        if (file == NULL || ctx == NULL) { return false; }
        unsigned char data_read[kBufferSize];
        size_t remainder_size = chunk_size % kBufferSize;
        //make sure the file ptr is from the first byte of icon content.
        if (file.Seek(base::File::FROM_BEGIN, chunk_offset) == kInvalidSetFilePointer){
            return false;
        }
        for (size_t i = 0; i < total_body_size / kBufferSize; i++) {
            if (file.ReadAtCurrentPos(data_read, kBufferSize) <= 0) { return false; }
            //Hash buffer
            if (1 != EVP_DigestUpdate(ctx, data_read, kBufferSize)){ return false; }
        }
        if (file.ReadAtCurrentPos(data_read, remainder_size) < remainder_size) { return false; }
        //Hash remainder
        if (1 != EVP_DigestUpdate(ctx, data_read, remainder_size)){ return false; }
        return true;
    }

    //Note: See https://wiki.fileformat.com/image/ico/#header
    //All values in ICO/CUR files are represented in little-endian byte order.
    bool PKCS7Ico::getFileContentDigest(EVP_MD* in_digest_algorithm, size_t &out_file_content_digest_size, uint8_t *out_file_content_digest){
        
        //Param check
        if (out_file_content_digest_size == NULL || out_file_content_digest == NULL) { return false; }
        bssl::ScopedEVP_MD_CTX mdctx;
        base::File file = getFile();
        //1.Init 
        if (1 != EVP_DigestInit_ex(mdctx.get(), in_digest_algorithm, NULL)){ return false; }
        //2.Update
        //Hash header
        HashFileHeader(file, ctx);
        //Hash content
        HashFileBody(file, ctx);
        //3.Final
        if (1 != EVP_DigestFinal_ex(mdctx.get(), out_file_content_digest, out_file_content_digest_size)){ return false; }
        return true;        
    }

}