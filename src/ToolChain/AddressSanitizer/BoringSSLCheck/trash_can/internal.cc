#include "internal.h"


// 1.2.840.113549.1.7.2
const uint8_t kPKCS7SignedData[9] = {0x2a, 0x86, 0x48, 0x86, 0xf7,
                                           0x0d, 0x01, 0x07, 0x02};


//pass
int PKCS7ParseHeader(uint8_t **der_bytes, CBS *out, CBS *cbs) {
  CBS in, content_info, content_type, wrapped_signed_data, signed_data;
  uint64_t version;

  // The input may be in BER format.
  *der_bytes = NULL;
  if (!CBS_asn1_ber_to_der(cbs, &in, der_bytes) ||
      // See https://tools.ietf.org/html/rfc2315#section-7
      !CBS_get_asn1(&in, &content_info, CBS_ASN1_SEQUENCE) ||
      !CBS_get_asn1(&content_info, &content_type, CBS_ASN1_OBJECT)) {
    return 0;
  }

  if (!CBS_mem_equal(&content_type, kPKCS7SignedData,
                     sizeof(kPKCS7SignedData))) {
    return 0;
  }

  // See https://tools.ietf.org/html/rfc2315#section-9.1
  if (!CBS_get_asn1(&content_info, &wrapped_signed_data,
                    CBS_ASN1_CONTEXT_SPECIFIC | CBS_ASN1_CONSTRUCTED | 0) ||
      !CBS_get_asn1(&wrapped_signed_data, &signed_data, CBS_ASN1_SEQUENCE) ||
      !CBS_get_asn1_uint64(&signed_data, &version) ||
      !CBS_get_asn1(&signed_data, NULL /* digests */, CBS_ASN1_SET) ||
      !CBS_get_asn1(&signed_data, NULL /* content */, CBS_ASN1_SEQUENCE)) {
    return 0;
  }
  if (version < 1) {	
	    return 0;	
  }
  CBS_init(out, CBS_data(&signed_data), CBS_len(&signed_data));
  return 1;
}