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

namespace sha {
    std::string sha256(const std::string& string);
}
#endif //BLOCKCHAIN_SHA256_H