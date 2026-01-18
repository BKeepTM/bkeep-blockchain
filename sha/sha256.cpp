#include "sha256.h"

//rais header file drugace multiple definitions....


// neki shit od stack overflow...
std::string sha::sha256(const std::string &string) {
    OpenSSLPointer<EVP_MD_CTX> context(EVP_MD_CTX_new());

    if(context.get() == NULL) {
        return "";
    }

    if(!EVP_DigestInit_ex(context.get(), EVP_sha256(), NULL)) {
        return "";
    }

    if(!EVP_DigestUpdate(context.get(), string.c_str(), string.length())) {
        return "";
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;

    if(!EVP_DigestFinal_ex(context.get(), hash, &lengthOfHash)) {
        return "";
    }

    std::stringstream ss;
    for(unsigned int i = 0; i < lengthOfHash; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    std::string hashed = ss.str();
    return hashed;
}
