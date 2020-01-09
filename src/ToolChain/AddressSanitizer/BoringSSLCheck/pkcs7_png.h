#ifndef CRYPTO_PKCS7_PNG_H_
#define CRYPTO_PKCS7_PNG_H_

#include "build/build_config.h"
#include "crypto/crypto_export.h"
#include "base/win/scoped_handle.h"
#include "base/base_paths.h"
#include "base/files/file.h"
#include "base/files/file_enumerator.h"
#include "base/files/file_path.h"
#include "base/files/scoped_file.h"
#include "base/files/scoped_temp_dir.h"
#include "base/path_service.h"


namespace crypto {
 
//Input scoped_fio.
class CRYPTO_EXPORT PKCS7Png {
 public:
   PKCS7Png(base::FilePath file_path);
   ~PKCS7Png();
   int64_t getFileSize();
   base::File getFile();  
   bool getSignedFileDigest(size_t &file_digest_size, uint8_t *file_digest);
   bool getFileContentDigest(size_t &out_file_content_digest_size, uint8_t *out_file_content_digest);

 private:
    base::FilePath file_path_;
    bool hashFileContent(base::File* inout_file, EVP_MD_CTX* inout_ctx);
};

}  // namespace crypto

#endif  // CRYPTO_PKCS7_PNG_H_
