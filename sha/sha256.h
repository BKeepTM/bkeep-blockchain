//
// Created by nejcsorgo on 12/29/2025.
//

#ifndef BLOCKCHAIN_SHA256_H
#define BLOCKCHAIN_SHA256_H
#include <openssl/evp.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
struct OpenSSLFree {
    void operator()(void* ptr) const {
        EVP_MD_CTX_free(static_cast<EVP_MD_CTX *>(ptr));
    }
};

template <typename T>
using OpenSSLPointer = std::unique_ptr<T, OpenSSLFree>;

class sha {
public:
    std::string sha256(const std::string& string);
};
#endif //BLOCKCHAIN_SHA256_H