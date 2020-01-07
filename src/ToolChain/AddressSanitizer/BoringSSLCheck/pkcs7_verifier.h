	#ifndef CRYPTO_PKCS7_VERIFIER_H_	
	#define CRYPTO_PKCS7_VERIFIER_H_	

	#include "pch.h"
		
	namespace crypto {	
		
	//#define PKCS7_type_is_signed(a) (OBJ_obj2nid((a)->type) == NID_pkcs7_signed)	
	class PKCS7Verifier {	
	 public:	
	    PKCS7Verifier(const unsigned char* data , size_t len);	
	    ~PKCS7Verifier();	
	    static int PKCS7ParseHeader(uint8_t **der_bytes, CBS *out, CBS *cbs);	
	    std::unique_ptr<CBS> SignedData();	
	    void ResetSignedData(const unsigned char* data, size_t len);	
	    /**	
	     * Message Digest Verification	
	     */	
	    bool PKCS7GetMessageDigestValue(CBS* in_signed_data, std::vector<uint8_t>& out_message_digest);	
	    bool PKCS7GetContentInfo(CBS* in_signed_data, std::vector<uint8_t>& out_content_info);	
	    bool PKCS7EVPMessageDigest(const unsigned char* message, size_t message_len, uint8_t digest[], unsigned int* digest_len);
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