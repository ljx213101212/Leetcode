#include "pkcs7_ico.h"
#include "pkcs7_constant.h"

namespace crypto{
    
    // static const size_t kPNGHeaderSize = 8;
    // static const long kInvalidSetFilePointer = -1;
    // static const size_t kBufferSize = 0x1000;
    // static const std::string kPNGSigChunkType("iTXt");
    // static const std::string kPNGIEndChunkType("IEND");
    // static const size_t kPNGChunkTypeSize = 4;
    // static const unsigned char kPngSignature[kPNGHeaderSize] = { 0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A};

    PKCS7Ico::PKCS7Ico(base::FilePath file_path){
        file_path_ = file_path;
    }
    PKCS7Ico::~PKCS7Ico() = default;
    
    size_t PKCS7Ico::GetFileCurrentPos(base::File& file){
        return file.Seek(base::File::FROM_CURRENT, 0);
    }

    base::File PKCS7Ico::getFile(){
        return base::File(file_path_, base::File::FLAG_OPEN | base::File::FLAG_READ);
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
            }
    
            // Set the internal pointers appropriately
        }
        info->png_end_position = GetFileCurrentPos(file);
        return true;
    }

    bool PKCS7Ico::getSignedFileDigest(size_t &file_digest_size, uint8_t *file_digest){

        IcoFileInfo info;
        if (!GetIcoFileInfo(&info)){
            return false;
        }
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

}