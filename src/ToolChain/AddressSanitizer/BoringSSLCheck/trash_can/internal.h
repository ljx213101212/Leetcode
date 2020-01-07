#include "pch.h"
#include <bytestring/internal.h>

extern const uint8_t kPKCS7SignedData[9];
extern int PKCS7ParseHeader(uint8_t **der_bytes, CBS *out, CBS *cbs);
static const size_t HASH_DIGEST_LENGTH = 40;