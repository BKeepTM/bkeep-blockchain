//
// Created by nejcsorgo on 12/23/2025.
//

#ifndef BLOCKCHAIN_BLOCK_H
#define BLOCKCHAIN_BLOCK_H
#include <cstdint>
#include <string>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

class Block {
    Block(int index, std::string previousHash,uint32_t timestamp, std::string data, int difficulty, int token);
    v
};


#endif //BLOCKCHAIN_BLOCK_H