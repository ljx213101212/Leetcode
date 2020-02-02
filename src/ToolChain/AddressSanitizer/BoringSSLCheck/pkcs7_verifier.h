#ifndef CRYPTO_PKCS7_VERIFIER_H_
#define CRYPTO_PKCS7_VERIFIER_H_

#include <stddef.h>
#include <stdint.h>
#include "build/build_config.h"
#include "crypto/crypto_export.h"
#include "crypto/openssl_util.h"
#include "crypto/signature_verifier.h"
#include "pkcs7_constant.h"

namespace crypto {

class CRYPTO_EXPORT PKCS7Verifier {
public:
	PKCS7Verifier(const uint8_t* data , size_t len);	
	~PKCS7Verifier();
	static int PKCS7ParseHeader(uint8_t** der_bytes, CBS* out_cbs, CBS* in_cbs);
	static const EVP_MD *cbs_to_md(const CBS *cbs);
	CBS GetSignedData();	
	void ResetSignedData(const unsigned char* in_data, size_t in_len);	

	/**
	* File Content Digest Verification. 
	*/
	bool PKCS7GetSpcIndirectDataContentDigestValue(CBS* in_signed_data, CBS* out_digest);
	bool PKCS7FileContentDigestVerification(CBS* in_signed_data, CBS* in_file_digest); 	

	/**	
	* Message Digest Verification	
	*/	
	bool PKCS7GetDigestAlgorithm(CBS* in_signed_data, CBS* out_cbs);
	bool PKCS7GetMessageDigestValue(CBS* in_signed_data, std::vector<uint8_t>& out_message_digest);	
	bool PKCS7GetContentInfo(CBS* in_signed_data, std::vector<uint8_t>& out_content_info);	
	bool PKCS7EVPMessageDigest(const EVP_MD* in_digest_algorithm, const unsigned char* in_message, size_t in_message_len, uint8_t out_digest[], unsigned int* out_digest_len);
	bool PKCS7MessageDigestValidation(CBS* in_signed_data , const EVP_MD* in_digest_algorithm);	
	/**	
	* Signature Verification	
	*/	
	bool PKCS7GetSignatureAlgorithm(CBS* in_public_key_info, const EVP_MD* in_signature_algorithm, SignatureVerifier::SignatureAlgorithm& out_algorithm);
	bool PKCS7GetPublicKeyInfo(CBS* in_signed_data, std::vector<uint8_t>& out_public_key_info);	
	bool PKCS7GetSignerInfo(CBS * in_signed_data, std::vector<uint8_t>& out_signer_info);	
	bool PKCS7GetEncryptedDigest(CBS * in_signed_data, std::vector<uint8_t>& out_encrypted_digest);	
private:	   	
	std::vector<uint8_t> signed_data_;	
};
}  // namespace crypto
#endif  // CRYPTO_PKCS7_VERIFIER_H_
