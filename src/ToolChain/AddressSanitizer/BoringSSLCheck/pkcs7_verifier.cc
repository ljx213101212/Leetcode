// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "crypto/pkcs7_verifier.h"
#include "third_party/boringssl/src/crypto/bytestring/internal.h"
#include "third_party/boringssl/src/crypto/internal.h"

namespace crypto{


PKCS7Verifier::PKCS7Verifier(const uint8_t* data , size_t len){		
    ResetSignedData(data, len);	
}

PKCS7Verifier::~PKCS7Verifier() = default;   

//static
//See boringssl/src/crypto/pkcs7/pkcs7.c
int PKCS7Verifier::PKCS7ParseHeader(uint8_t** der_bytes, CBS* out_cbs, CBS* in_cbs){	
    
    CBS in, content_info, content_type, wrapped_signed_data, signed_data;	
    uint64_t version;	
    // The input may be in BER format.	
    *der_bytes = NULL;	
    if (!CBS_asn1_ber_to_der(in_cbs, &in, der_bytes) ||	
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
     // See Authenticode_PE, version must be 1.		
    if (version != 1) {	
    return 0;	
    }	
    CBS_init(out_cbs, CBS_data(&signed_data), CBS_len(&signed_data));	
    return 1;	
}

//static
const EVP_MD *PKCS7Verifier::cbs_to_md(const CBS *cbs)
{
    for (size_t i = 0; i < kMDOIDsVector.size(); i++)
    {
        if (CBS_len(cbs) == kMDOIDsVector[i].oid_len &&
            memcmp(CBS_data(cbs), kMDOIDsVector[i].oid, kMDOIDsVector[i].oid_len) ==
                0)
        {
            return EVP_get_digestbynid(kMDOIDsVector[i].nid);
        }
    }
    return NULL;
}
//Reset signed data.    
void PKCS7Verifier::ResetSignedData(const unsigned char* in_data, size_t in_len){	
    signed_data_.assign(in_data, in_data + in_len);
}	

// CBS member -> *data address changes by executing CBS_get_asn1, so need a refresh method to reload the ptr back to start.
// Get new signed data CBS (Reset data chunk ptr to start)
CBS PKCS7Verifier::GetSignedData(){	
    CBS signed_data;	
    CBS_init(&signed_data, signed_data_.data(), signed_data_.size());	
    return signed_data;	
}	

bool PKCS7Verifier::PKCS7GetSpcIndirectDataContentDigestValue(CBS* in_signed_data, CBS* out_digest){

    std::unique_ptr<uint8_t*> der_bytes = std::make_unique<uint8_t*>();	
    CBS in, content_info, content_type, wrapped_signed_data, signed_data;	
    CBS spc_indirect_data_wrapper, spc_indirect_data_content_type, wrappered_spc_indirect_data, content_info_value, spc_indirect_data_content_digest, spc_indirect_data_content_digest_value;	
    uint64_t version;	
    
    if (!CBS_asn1_ber_to_der(in_signed_data, &in, der_bytes.get()) ||	
        // See https://tools.ietf.org/html/rfc2315#section-7	
        !CBS_get_asn1(&in, &content_info, CBS_ASN1_SEQUENCE) ||	
        !CBS_get_asn1(&content_info, &content_type, CBS_ASN1_OBJECT)) {	
        return false;	
    }	
    if (!CBS_mem_equal(&content_type, kPKCS7SignedData,	
        sizeof(kPKCS7SignedData))) {	
        return false;	
    }	
    // See https://tools.ietf.org/html/rfc2315#section-9.1	
    if(!CBS_get_asn1(&content_info, &wrapped_signed_data,	
        CBS_ASN1_CONTEXT_SPECIFIC | CBS_ASN1_CONSTRUCTED | 0)){return false;}	
    if(!CBS_get_asn1(&wrapped_signed_data, &signed_data, CBS_ASN1_SEQUENCE)){return false;}	
    if(!CBS_get_asn1_uint64(&signed_data, &version)){return false;}	
    if(!CBS_get_asn1(&signed_data, NULL, CBS_ASN1_SET)){return false;}	
    if(!CBS_get_asn1(&signed_data, &spc_indirect_data_wrapper, CBS_ASN1_SEQUENCE)){return false;}	
    if(!CBS_get_asn1(&spc_indirect_data_wrapper, &spc_indirect_data_content_type, CBS_ASN1_OBJECT)){return false;}	
    if(!CBS_get_asn1(&spc_indirect_data_wrapper, &wrappered_spc_indirect_data,	
        CBS_ASN1_CONTEXT_SPECIFIC | CBS_ASN1_CONSTRUCTED | 0)){return false;}	
    if(!CBS_get_asn1(&wrappered_spc_indirect_data, &content_info_value, CBS_ASN1_SEQUENCE)){return false;}	
    if(!CBS_get_asn1(&content_info_value, NULL, CBS_ASN1_SEQUENCE)){return false;}
    if(!CBS_get_asn1(&content_info_value, &spc_indirect_data_content_digest, CBS_ASN1_SEQUENCE)){return false;}
    if(!CBS_get_asn1(&spc_indirect_data_content_digest, NULL, CBS_ASN1_SEQUENCE)){return false;}
    if(!CBS_get_asn1(&spc_indirect_data_content_digest, &spc_indirect_data_content_digest_value, CBS_ASN1_OCTETSTRING)){return false;}
    // See Authenticode_PE, version must be 1.	
    if (version != 1) {	
        return false;	
    }
    CBS_init(out_digest, CBS_data(&spc_indirect_data_content_digest_value), CBS_len(&spc_indirect_data_content_digest_value));
    return true;	
}

bool PKCS7Verifier::PKCS7FileContentDigestVerification(CBS* in_signed_data, CBS* in_file_digest) {
    //Get digest from signed_data (signature)
    CBS spc_indirect_data_content_digest;
    PKCS7GetSpcIndirectDataContentDigestValue(in_signed_data, &spc_indirect_data_content_digest);
    //compare length
    bool is_length_ok = CBS_len(&spc_indirect_data_content_digest) == CBS_len(in_file_digest);
    //compare content
    bool is_content_ok = memcmp(CBS_data(&spc_indirect_data_content_digest),CBS_data(in_file_digest), CBS_len(in_file_digest));
    //Both equal then OK
    return is_length_ok && is_content_ok;
}
bool PKCS7Verifier::PKCS7GetDigestAlgorithm(CBS* in_signed_data, CBS* out_cbs) {
     std::unique_ptr<uint8_t*> der_bytes = std::make_unique<uint8_t*>();	
    CBS in, content_info, content_type, wrapped_signed_data, signed_data;	
    CBS spc_indirect_data_wrapper, spc_indirect_data_content_type, wrappered_spc_indirect_data, content_info_value, wrappered_digest_algo, digest_algo, digest_algo_value;	
    uint64_t version;	
    
    if (!CBS_asn1_ber_to_der(in_signed_data, &in, der_bytes.get()) ||	
        // See https://tools.ietf.org/html/rfc2315#section-7	
        !CBS_get_asn1(&in, &content_info, CBS_ASN1_SEQUENCE) ||	
        !CBS_get_asn1(&content_info, &content_type, CBS_ASN1_OBJECT)) {	
        return false;	
    }	
    if (!CBS_mem_equal(&content_type, kPKCS7SignedData,	
        sizeof(kPKCS7SignedData))) {	
        return false;	
    }	
    // See https://tools.ietf.org/html/rfc2315#section-9.1	
    if(!CBS_get_asn1(&content_info, &wrapped_signed_data,	
        CBS_ASN1_CONTEXT_SPECIFIC | CBS_ASN1_CONSTRUCTED | 0)){return false;}	
    if(!CBS_get_asn1(&wrapped_signed_data, &signed_data, CBS_ASN1_SEQUENCE)){return false;}	
    if(!CBS_get_asn1_uint64(&signed_data, &version)){return false;}	
    if(!CBS_get_asn1(&signed_data, NULL /* digests */, CBS_ASN1_SET)){return false;}	
    if(!CBS_get_asn1(&signed_data, &spc_indirect_data_wrapper /* content */, CBS_ASN1_SEQUENCE)){return false;}	
    if(!CBS_get_asn1(&spc_indirect_data_wrapper, &spc_indirect_data_content_type /* content */, CBS_ASN1_OBJECT)){return false;}	
    if(!CBS_get_asn1(&spc_indirect_data_wrapper, &wrappered_spc_indirect_data,	
        CBS_ASN1_CONTEXT_SPECIFIC | CBS_ASN1_CONSTRUCTED | 0)){return false;}	
    if(!CBS_get_asn1(&wrappered_spc_indirect_data, &content_info_value /* content */, CBS_ASN1_SEQUENCE)){return false;}
    if(!CBS_get_asn1(&content_info_value, NULL, CBS_ASN1_SEQUENCE)){return false;}	
    if(!CBS_get_asn1(&content_info_value, &wrappered_digest_algo, CBS_ASN1_SEQUENCE)){return false;}
    if(!CBS_get_asn1(&wrappered_digest_algo, &digest_algo, CBS_ASN1_SEQUENCE)){return false;}
    if(!CBS_get_asn1(&digest_algo, &digest_algo_value, CBS_ASN1_OBJECT)){return false;}
    // See Authenticode_PE, version must be 1.	
    if (version != 1) {	
        return false;	
    }
    CBS_init(out_cbs, CBS_data(&digest_algo_value), CBS_len(&digest_algo_value));
    return true;	
}

//See https://tools.ietf.org/html/rfc2315#section-9.2	
//See 1.2.840.113549.1.9.4 Authenticode_PE	
//EncryptedDigest	
bool PKCS7Verifier::PKCS7GetMessageDigestValue(CBS* in_signed_data, std::vector<uint8_t>& out_message_digest){	
    
    std::unique_ptr<uint8_t*> der_bytes = std::make_unique<uint8_t*>();	
    CBS signed_data, signed_data_seq, 	
        signed_data_seq_inner, signer_info, message_digest_wrapper, message_digest_set, message_digest_value;	
    
    if (!PKCS7ParseHeader(der_bytes.get(), &signed_data, in_signed_data) ||	
        !CBS_get_optional_asn1(	
            &signed_data, NULL, NULL,	
            CBS_ASN1_CONTEXT_SPECIFIC | CBS_ASN1_CONSTRUCTED | 0)) {	
        return false;	
    }	
    if(!CBS_get_asn1(&signed_data, &signed_data_seq, CBS_ASN1_SET)){ return false;}	
    if(!CBS_get_asn1(&signed_data_seq, &signed_data_seq_inner, CBS_ASN1_SEQUENCE)){ return false; }	
    if(!CBS_get_asn1(&signed_data_seq_inner, NULL, CBS_ASN1_INTEGER)){ return false; }	
    if(!CBS_get_asn1(&signed_data_seq_inner, NULL, CBS_ASN1_SEQUENCE)){ return false; }	
    if(!CBS_get_asn1(&signed_data_seq_inner, NULL, CBS_ASN1_SEQUENCE)){ return false; }	
    if(!CBS_get_optional_asn1(	
        &signed_data_seq_inner, &signer_info, NULL,	
        CBS_ASN1_CONTEXT_SPECIFIC | CBS_ASN1_CONSTRUCTED | 0)) { return false;}	
    if(!CBS_get_asn1(&signer_info, NULL, CBS_ASN1_SEQUENCE)){ return false;}	
    if(!CBS_get_asn1(&signer_info, NULL, CBS_ASN1_SEQUENCE)){ return false;}	
    if(!CBS_get_asn1(&signer_info, &message_digest_wrapper, CBS_ASN1_SEQUENCE)){return false;}	
    if(!CBS_get_asn1(&message_digest_wrapper, NULL, CBS_ASN1_OBJECT)) { return false; }	
    if(!CBS_get_asn1(&message_digest_wrapper, &message_digest_set, CBS_ASN1_SET)){return false;}	
    if(!CBS_get_asn1(&message_digest_set, &message_digest_value, CBS_ASN1_OCTETSTRING)){return false;}	
    const uint8_t* tmp = CBS_data(&message_digest_value);	
    size_t out_size =  CBS_len(&message_digest_value);	
    out_message_digest.resize(out_size);	
    out_message_digest.assign(tmp,(tmp + out_size));	
    return true;	
}	
    

bool PKCS7Verifier::PKCS7GetContentInfo(CBS* in_signed_data, std::vector<uint8_t>& out_content_info){	

    std::unique_ptr<uint8_t*> der_bytes = std::make_unique<uint8_t*>();	
    CBS in, content_info, content_type, wrapped_signed_data, signed_data;	
    CBS spc_indirect_data_wrapper, spc_indirect_data_content_type, wrappered_spc_indirect_data, content_info_value;	
    uint64_t version;	
    
    if (!CBS_asn1_ber_to_der(in_signed_data, &in, der_bytes.get()) ||	
        // See https://tools.ietf.org/html/rfc2315#section-7	
        !CBS_get_asn1(&in, &content_info, CBS_ASN1_SEQUENCE) ||	
        !CBS_get_asn1(&content_info, &content_type, CBS_ASN1_OBJECT)) {	
        return false;	
    }	
    if (!CBS_mem_equal(&content_type, kPKCS7SignedData,	
        sizeof(kPKCS7SignedData))) {	
        return false;	
    }	
    // See https://tools.ietf.org/html/rfc2315#section-9.1	
    if(!CBS_get_asn1(&content_info, &wrapped_signed_data,	
        CBS_ASN1_CONTEXT_SPECIFIC | CBS_ASN1_CONSTRUCTED | 0)){return false;}	
    if(!CBS_get_asn1(&wrapped_signed_data, &signed_data, CBS_ASN1_SEQUENCE)){return false;}	
    if(!CBS_get_asn1_uint64(&signed_data, &version)){return false;}	
    if(!CBS_get_asn1(&signed_data, NULL /* digests */, CBS_ASN1_SET)){return false;}	
    if(!CBS_get_asn1(&signed_data, &spc_indirect_data_wrapper /* content */, CBS_ASN1_SEQUENCE)){return false;}	
    if(!CBS_get_asn1(&spc_indirect_data_wrapper, &spc_indirect_data_content_type /* content */, CBS_ASN1_OBJECT)){return false;}	
    if(!CBS_get_asn1(&spc_indirect_data_wrapper, &wrappered_spc_indirect_data,	
        CBS_ASN1_CONTEXT_SPECIFIC | CBS_ASN1_CONSTRUCTED | 0)){return false;}	
    if(!CBS_get_asn1(&wrappered_spc_indirect_data, &content_info_value /* content */, CBS_ASN1_SEQUENCE)){return false;}	
    
    // See Authenticode_PE, version must be 1.	
    if (version != 1) {	
        return false;	
    }	
    const uint8_t* tmp = CBS_data(&content_info_value);	
    size_t out_size =  CBS_len(&content_info_value);	
    out_content_info.resize(out_size);	
    out_content_info.assign(tmp,(tmp + out_size));	
    return true;	
}	

//See boringssl/src/crypto/digest_test.cc (reference)
bool PKCS7Verifier::PKCS7EVPMessageDigest(const EVP_MD* in_digest_algorithm, const unsigned char* in_message, size_t in_message_len, uint8_t out_digest[], unsigned int* out_digest_len){

    bssl::ScopedEVP_MD_CTX mdctx;
    if (1 != EVP_DigestInit_ex(mdctx.get(), in_digest_algorithm, NULL)){ return false; }
    if (1 != EVP_DigestUpdate(mdctx.get(), in_message, in_message_len)){ return false; }
    if (1 != EVP_DigestFinal_ex(mdctx.get(), out_digest, out_digest_len)){ return false; }
    return true;
}   	

//Compare message_digest from signed_data and Digested Content Info (size compare & memcmp)
//See https://tools.ietf.org/html/rfc2315#section-9.3 
/*
   Specifically, the initial input is the contents octets of the DER
   encoding of the content field of the ContentInfo value to which the
   signing process is applied. Only the contents octets of the DER
   encoding of that field are digested, not the identifier octets or the
   length octets.
*/
bool PKCS7Verifier::PKCS7MessageDigestValidation(CBS* in_signed_data , const EVP_MD* in_digest_algorithm){

    CBS cbs_input;
    CBS_init(&cbs_input, CBS_data(in_signed_data), CBS_len(in_signed_data));	
    //1.Get message digest from SignerInfo 	
    std::vector<uint8_t> message_digest;	
    PKCS7GetMessageDigestValue(&cbs_input, message_digest);	
    //refresh cbs_input	
    CBS_init(&cbs_input, CBS_data(in_signed_data), CBS_len(in_signed_data));	
    //2.Get Content Info	
    std::vector<uint8_t> content_info;	
    PKCS7GetContentInfo(&cbs_input, content_info);	
    //refresh cbs_input	
    CBS_init(&cbs_input, CBS_data(in_signed_data), CBS_len(in_signed_data));	
    //3.Get Content Info Message Digest	
    std::unique_ptr<unsigned char[]>digest = std::make_unique<unsigned char[]>(HASH_DIGEST_LENGTH);	
    unsigned int digest_len;	
    PKCS7EVPMessageDigest(in_digest_algorithm, content_info.data(), content_info.size(),digest.get(), &digest_len);
    //Compare size and content	
    int is_md_Ok = (digest_len == message_digest.size() &&	
                    (0 == memcmp(digest.get(), message_digest.data(), digest_len)));	
    return (bool)is_md_Ok;	
}	


bool PKCS7Verifier::PKCS7GetSignatureAlgorithm(CBS* in_public_key_info, const EVP_MD* in_signature_algorithm, SignatureVerifier::SignatureAlgorithm& out_algorithm){
    //param check
    if (in_public_key_info == NULL) { return false; }
    bssl::UniquePtr<EVP_PKEY> public_key(EVP_parse_public_key(in_public_key_info));
    int public_key_type = EVP_PKEY_id(public_key.get());
    int md_type = EVP_MD_type(in_signature_algorithm);
    
    if (public_key_type == EVP_PKEY_RSA
       && md_type == NID_sha256){
        out_algorithm = SignatureVerifier::SignatureAlgorithm::RSA_PKCS1_SHA256;
    }else if (public_key_type == EVP_PKEY_RSA
      && md_type == NID_sha1) {
        out_algorithm = SignatureVerifier::SignatureAlgorithm::RSA_PKCS1_SHA1;
    }else if (public_key_type == EVP_PKEY_EC
      && md_type == NID_sha256){
        out_algorithm = SignatureVerifier::SignatureAlgorithm::ECDSA_SHA256;
    }else{
        //NOT SUPPORTED signature algorithm
        return false;
    }
    return true;
}
 //see https://tools.ietf.org/html/rfc5280 4.1 Basic Certificate Fields
 //subjectPublicKeyInfo 
 bool PKCS7Verifier::PKCS7GetPublicKeyInfo(CBS* in_signed_data, std::vector<uint8_t>& out_public_key_info){
    std::unique_ptr<uint8_t*> der_bytes = std::make_unique<uint8_t*>();
    CBS signed_data, certificates, public_key_info, toplevel;
	  if (!PKCS7ParseHeader(der_bytes.get(), &signed_data, in_signed_data) ||
		!CBS_get_optional_asn1(
			&signed_data, &certificates, NULL,
			CBS_ASN1_CONTEXT_SPECIFIC | CBS_ASN1_CONSTRUCTED | 0)) {
	    return false;
	  }
	  if (!CBS_get_asn1(&certificates, &toplevel, CBS_ASN1_SEQUENCE)) { return false; }
	  if (!CBS_get_asn1(&toplevel, &public_key_info, CBS_ASN1_SEQUENCE)) { return false; }
	  if (!CBS_get_optional_asn1(
		&public_key_info, NULL, NULL,
		CBS_ASN1_CONSTRUCTED | CBS_ASN1_CONTEXT_SPECIFIC | 0)) { return false; }
	  if (!CBS_get_asn1(&public_key_info, NULL, CBS_ASN1_INTEGER)) { return false; }
	  if (!CBS_get_asn1(&public_key_info, NULL, CBS_ASN1_SEQUENCE)) { return false; }
	  if (!CBS_get_asn1(&public_key_info, NULL, CBS_ASN1_SEQUENCE)) { return false; }
	  if (!CBS_get_asn1(&public_key_info, NULL, CBS_ASN1_SEQUENCE)) { return false; }
	  if (!CBS_get_asn1(&public_key_info, NULL, CBS_ASN1_SEQUENCE)) { return false; }

    uint8_t* tmp = (uint8_t*)CBS_data(&public_key_info);
    size_t out_size =  CBS_len(&public_key_info);
    out_public_key_info.resize(out_size);
    out_public_key_info.assign(tmp,(tmp + out_size));
	  return true;
  }


//See https://tools.ietf.org/html/rfc5280 4.1 Basic Certificate Fields
//See Authenticode_PE #SignerInfo
bool PKCS7Verifier::PKCS7GetSignerInfo(CBS * in_signed_data, std::vector<uint8_t>& out_signer_info){	
std::unique_ptr<uint8_t*> der_bytes = std::make_unique<uint8_t*>();	
    CBS signed_data, signed_data_seq, signed_data_seq_inner, signer_info;	
    if (!PKCS7ParseHeader(der_bytes.get(), &signed_data, in_signed_data) ||	
    !CBS_get_optional_asn1(	
        &signed_data, NULL , NULL,	
        CBS_ASN1_CONTEXT_SPECIFIC | CBS_ASN1_CONSTRUCTED | 0)) {	
    return false;	
    }	
    if (!CBS_get_asn1(&signed_data, &signed_data_seq, CBS_ASN1_SET)) { return false; }	
    if (!CBS_get_asn1(&signed_data_seq, &signed_data_seq_inner, CBS_ASN1_SEQUENCE)) { return false; }	
    if (!CBS_get_asn1(&signed_data_seq_inner, NULL, CBS_ASN1_INTEGER)) { return false; }	
    if (!CBS_get_asn1(&signed_data_seq_inner, NULL, CBS_ASN1_SEQUENCE)) { return false; }	
    if (!CBS_get_asn1(&signed_data_seq_inner, NULL, CBS_ASN1_SEQUENCE)) { return false; }	
    if (!CBS_get_optional_asn1(	
    &signed_data_seq_inner, &signer_info, NULL,	
    CBS_ASN1_CONTEXT_SPECIFIC | CBS_ASN1_CONSTRUCTED | 0)) { return false; }	

    const uint8_t* tmp = CBS_data(&signer_info);	
    size_t out_size =  CBS_len(&signer_info);	
    out_signer_info.resize(out_size);	
    out_signer_info.assign(tmp,(tmp + out_size));	

    //See Openssl source: asn1_template_ex_i2d - ASN1_put_object	
    //See http://luca.ntop.org/Teaching/Appunti/asn1.html	
    //See https://en.wikipedia.org/wiki/X.690#Identifier_octets	
    //See https://community.rsa.com/docs/DOC-51344 - 	
    //0x31 (which denotes a SET OF) instead of A0 (which is the context-specific tag)	
    std::vector<uint8_t> prefix = { 0x31, (uint8_t)out_size };	
    out_signer_info.insert(out_signer_info.begin(), prefix.begin(), prefix.end());	
    return true;	
}	

//See https://tools.ietf.org/html/rfc2315#section-13
//See https://tools.ietf.org/html/rfc2315#section-10
bool PKCS7Verifier::PKCS7GetEncryptedDigest(CBS * in_signed_data, std::vector<uint8_t>& out_encrypted_digest){	
    
    std::unique_ptr<uint8_t*> der_bytes = std::make_unique<uint8_t*>();	
    CBS signed_data, certificates, signed_data_seq, signed_data_seq_inner, octet_string;	
    if (!PKCS7ParseHeader(der_bytes.get(), &signed_data, in_signed_data) ||	
        !CBS_get_optional_asn1(	
        &signed_data, &certificates, NULL,	
        CBS_ASN1_CONTEXT_SPECIFIC | CBS_ASN1_CONSTRUCTED | 0)) {	
    return false;	
    }	
    if(!CBS_get_asn1(&signed_data, &signed_data_seq, CBS_ASN1_SET)) { return false; }	
    if(!CBS_get_asn1(&signed_data_seq, &signed_data_seq_inner, CBS_ASN1_SEQUENCE)) { return false; }	
    if(!CBS_get_asn1(&signed_data_seq_inner, NULL, CBS_ASN1_INTEGER)) { return false; }	
    if(!CBS_get_asn1(&signed_data_seq_inner, NULL, CBS_ASN1_SEQUENCE)) { return false; }	
    if(!CBS_get_asn1(&signed_data_seq_inner, NULL, CBS_ASN1_SEQUENCE)) { return false; }	
    if(!CBS_get_optional_asn1(	
    &signed_data_seq_inner, NULL, NULL,	
    CBS_ASN1_CONTEXT_SPECIFIC | CBS_ASN1_CONSTRUCTED | 0)) { return false; }	
    if(!CBS_get_asn1(&signed_data_seq_inner, NULL, CBS_ASN1_SEQUENCE)) { return false; }	
    if(!CBS_get_asn1(&signed_data_seq_inner, &octet_string, CBS_ASN1_OCTETSTRING)) { return false; }	

    const uint8_t* tmp = CBS_data(&octet_string);	
    size_t out_size =  CBS_len(&octet_string);	
    out_encrypted_digest.resize(out_size);	
    out_encrypted_digest.assign(tmp,(tmp + out_size));	
    return true;	
}	
}