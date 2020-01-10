#ifndef CRYPTO_PKCS7_ICO_H_
#define CRYPTO_PKCS7_ICO_H_

#include "build/build_config.h"

#include "base/win/scoped_handle.h"
#include "base/base_paths.h"
#include "base/files/file.h"
#include "base/files/file_enumerator.h"
#include "base/files/file_path.h"
#include "base/files/scoped_file.h"
#include "base/files/scoped_temp_dir.h"
#include "base/path_service.h"
#include "crypto/crypto_export.h"


namespace crypto {

//See https://github.com/Springuin/Mailnotifier-ConfigurationTool/blob/master/iconeditor/4493/ICONS.C
typedef struct IcoFileInfo_
{
    size_t file_start_position;
    size_t png_start_position;
    size_t png_end_position;
    size_t nth_image_is_png;
    size_t num_of_ico;
    size_t png_chunk_offset;
    size_t png_chunk_size;
}IcoFileInfo;

typedef struct
{
	unsigned char	bWidth;               // Width of the image
	unsigned char	bHeight;              // Height of the image (times 2)
	unsigned char	bColorCount;          // Number of colors in image (0 if >=8bpp)
	unsigned char	bReserved;            // Reserved
	unsigned short	wPlanes;              // Color Planes
	unsigned short	wBitCount;            // Bits per pixel
	unsigned long	dwBytesInRes;         // how many bytes in this resource?
	unsigned long	dwImageOffset;        // where in the file is this image
} ICONDIRENTRY, * LPICONDIRENTRY;

// The following two structs are for the use of this program in
// manipulating icons. They are more closely tied to the operation
// of this program than the structures listed above. One of the
// main differences is that they provide a pointer to the DIB
// information of the masks.
typedef struct
{
	// unsigned int			Width, Height, Colors; // Width, Height and bpp
	std::unique_ptr<unsigned char far[]>			lpBits;                // ptr to DIB bits
	unsigned long			dwNumBytes;            // how many bytes?
	// LPBITMAPINFO	lpbi;                  // ptr to header
	// std::unique_ptr<unsigned char far>			lpXOR;                 // ptr to XOR image bits
	// std::unique_ptr<unsigned char far>			lpAND;                 // ptr to AND image bits
} ICONIMAGE, * LPICONIMAGE;
typedef struct
{
	// bool		bHasChanged;                     // Has image changed?
	// char		szOriginalICOFileName[MAX_PATH]; // Original name
	// char		szOriginalDLLFileName[MAX_PATH]; // Original name
	unsigned int		nNumImages;                      // How many images?
	std::unique_ptr<ICONIMAGE[]> IconImages;
	// ICONIMAGE	IconImages[1];                   // Image entries
} ICONRESOURCE, * LPICONRESOURCE;

//Input scoped_fio.
class CRYPTO_EXPORT PKCS7Ico {
public:
   PKCS7Ico(base::FilePath file_path);
   ~PKCS7Ico();
   static size_t GetFileCurrentPos(base::File& file); 
   int64_t getFileSize();
   base::File getFile();
   bool CheckPngSignature(unsigned char* chunkData); 
   int ReadIcoImageNumber();
   bool GetIcoFileInfo(IcoFileInfo *info); 
   bool getSignedFileDigest(size_t &file_digest_size, uint8_t *file_digest);
   bool getFileContentDigest(size_t &out_file_content_digest_size, uint8_t *out_file_content_digest);

 private:
    base::FilePath file_path_;

};

}  // namespace crypto

#endif  // CRYPTO_PKCS7_ICO_H_
