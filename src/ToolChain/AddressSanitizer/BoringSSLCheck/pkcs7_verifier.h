	#ifndef CRYPTO_PKCS7_VERIFIER_H_	
	#define CRYPTO_PKCS7_VERIFIER_H_	
	#include "pch.h"
		
	namespace crypto {	

	class PKCS7Verifier {	
	 public:	
	    PKCS7Verifier(const uint8_t* data , size_t len);	
	    ~PKCS7Verifier();
	    static int PKCS7ParseHeader(uint8_t** der_bytes, CBS* out_cbs, CBS* in_cbs);	
        CBS GetSignedData();	
	    void ResetSignedData(const unsigned char* in_data, size_t in_len);	
	    /**	
	     * Message Digest Verification	
	     */	
	    bool PKCS7GetMessageDigestValue(CBS* in_signed_data, std::vector<uint8_t>& out_message_digest);	
	    bool PKCS7GetContentInfo(CBS* in_signed_data, std::vector<uint8_t>& out_content_info);	
	    bool PKCS7EVPMessageDigest(const unsigned char* in_message, size_t in_message_len, uint8_t out_digest[], unsigned int* out_digest_len);
	    bool PKCS7MessageDigestValidation(CBS* in_signed_data);	
	    /**	
	     * Signature Verification	
	     */	
	   bool PKCS7GetPublicKeyInfo(CBS* in_signed_data, std::vector<uint8_t>& out_public_key_info);	
	   bool PKCS7GetSignerInfo(CBS * in_signed_data, std::vector<uint8_t>& out_signer_info);	
	   bool PKCS7GetEncryptedDigest(CBS * in_signed_data, std::vector<uint8_t>& out_encrypted_digest);	
	 private:	
	   	
	    std::vector<uint8_t> signed_data_;	
	};	
	}  // namespace crypto	
	#endif  // CRYPTO_PKCS7_VERIFIER_H_