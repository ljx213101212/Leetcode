// Copyright (c) 2011 The Chromium Authors. All rights reserved.	
	// Use of this source code is governed by a BSD-style license that can be	
	// found in the LICENSE file.
#include "pkcs7_constant.h"	
#include "pkcs7_verifier.h"	
#include <fstream>	
#include <string>	
#include <bytestring/internal.h>
		
namespace crypto{	
            
PKCS7Verifier::PKCS7Verifier(const uint8_t* data , size_t len){		
    ResetSignedData(data, len);	
}	
PKCS7Verifier::~PKCS7Verifier() = default;   	
    
//static
//See boringssl/src/crypto/pkcs7/pkcs7.c
int PKCS7Verifier::PKCS7ParseHeader(uint8_t** der_bytes, CBS* out, CBS*cbs){	
    
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

//Reset signed data.    
void PKCS7Verifier::ResetSignedData(const unsigned char* data, size_t len){	
    signed_data_.assign(data, data + len);
}	

// CBS member -> *data address changes by executing CBS_get_asn1, so need a refresh method to reload the ptr back to start.
// Get new signed data CBS (Reset data chunk ptr to start)
CBS PKCS7Verifier::GetSignedData(){	
    CBS signed_data;	
    CBS_init(&signed_data, signed_data_.data(), signed_data_.size());	
    return signed_data;	
}	
    
//See https://tools.ietf.org/html/rfc2315#section-9.2	
//See 1.2.840.113549.1.9.4 Authenticode_PE	
//EncryptedDigest	
bool PKCS7Verifier::PKCS7GetMessageDigestValue(CBS* in_signed_data, std::vector<uint8_t>& out_message_digest){	
    
    std::unique_ptr<uint8_t*> der_bytes = std::make_unique<uint8_t*>();	
    CBS signed_data, signed_data_seq, 	
        signed_data_seq_inner, signer_info, message_digest_wrapper, message_digest_set, ret;	
    
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
    if(!CBS_get_asn1(&message_digest_set, &ret, CBS_ASN1_OCTETSTRING)){return false;}	
    uint8_t* tmp = (uint8_t*)CBS_data(&ret);	
    size_t out_size =  CBS_len(&ret);	
    out_message_digest.resize(out_size);	
    out_message_digest.assign(tmp,(tmp + out_size));	
    return true;	
}	
    
    
    bool PKCS7Verifier::PKCS7GetContentInfo(CBS* in_signed_data, std::vector<uint8_t>& out_content_info){	
    
    std::unique_ptr<uint8_t*> der_bytes = std::make_unique<uint8_t*>();	
    CBS in, content_info, content_type, wrapped_signed_data, signed_data;	
    CBS spc_indirect_data_wrapper, spc_indirect_data_content_type, wrappered_spc_indirect_data, ret;	
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
    if(!CBS_get_asn1(&wrappered_spc_indirect_data, &ret /* content */, CBS_ASN1_SEQUENCE)){return false;}	
    
    // See Authenticode_PE, version must be 1.	
    if (version != 1) {	
        return false;	
    }	
    uint8_t* tmp = (uint8_t*)CBS_data(&ret);	
    size_t out_size =  CBS_len(&ret);	
    out_content_info.resize(out_size);	
    out_content_info.assign(tmp,(tmp + out_size));	
    return true;	
    }	
    
bool PKCS7Verifier::PKCS7EVPMessageDigest(const unsigned char* message, size_t message_len, uint8_t digest[], unsigned int* digest_len){	
        
    EVP_MD_CTX* mdctx;
    if ((mdctx = EVP_MD_CTX_create()) == NULL){
        EVP_MD_CTX_destroy(mdctx);
        return false;
    }
    if (1 != EVP_DigestInit_ex(mdctx, EVP_sha1(), NULL)){
        EVP_MD_CTX_destroy(mdctx);
        return false;
    }
    if (1 != EVP_DigestUpdate(mdctx, message, message_len)){
        EVP_MD_CTX_destroy(mdctx);
        return false;
    }
    if (1 != EVP_DigestFinal_ex(mdctx, digest, digest_len)){
        EVP_MD_CTX_destroy(mdctx);
        return false;
    }
    EVP_MD_CTX_destroy(mdctx);
    return true;
}	
    
    bool PKCS7Verifier::PKCS7MessageDigestValidation(CBS* in_signed_data){	
    std::unique_ptr<CBS> cbs_input;	
    cbs_input.reset(new CBS);	
    CBS* tmp_cbs = cbs_input.get();	
    CBS_init(tmp_cbs, CBS_data(in_signed_data), CBS_len(in_signed_data));	
    //1.Get message digest from SignerInfo 	
    std::vector<uint8_t> message_digest;	
    PKCS7GetMessageDigestValue(tmp_cbs, message_digest);	
    //refresh tmp_cbs	
    CBS_init(tmp_cbs, CBS_data(in_signed_data), CBS_len(in_signed_data));	
    //2.Get Content Info	
    std::vector<uint8_t> content_info;	
    PKCS7GetContentInfo(tmp_cbs, content_info);	
    //refresh tmp_cbs	
    CBS_init(tmp_cbs, CBS_data(in_signed_data), CBS_len(in_signed_data));	
    //3.Get Content Info Message Digest	
    std::unique_ptr<unsigned char[]>digest = std::make_unique<unsigned char[]>(HASH_DIGEST_LENGTH);	
        unsigned int digest_len;	
    PKCS7EVPMessageDigest(content_info.data(), content_info.size(),digest.get(), &digest_len);	
    int is_md_Ok = (digest_len == message_digest.size() &&	
                    (0 == memcmp(digest.get(), message_digest.data(), digest_len)));	
    return (bool)is_md_Ok;	
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
    
    
    //see https://tools.ietf.org/html/rfc5280 4.1 Basic Certificate Fields	
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
    
    uint8_t* tmp = (uint8_t*)CBS_data(&signer_info);	
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
    
    uint8_t* tmp = (uint8_t*)CBS_data(&octet_string);	
    size_t out_size =  CBS_len(&octet_string);	
    out_encrypted_digest.resize(out_size);	
    out_encrypted_digest.assign(tmp,(tmp + out_size));	
        return true;	
    }	
}