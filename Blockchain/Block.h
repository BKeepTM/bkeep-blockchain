#ifndef BLOCKCHAIN_BLOCK_H
#define BLOCKCHAIN_BLOCK_H
#include <cstdint>
#include <string>

#include "../sha/sha256.cpp"

class Block {
    public:
    Block(int index, std::string previousHash,time_t timestamp, std::string data, int difficulty, int token);
    std::string calculateHash();

    int index;
    int difficulty;
    int token;

    std::string hash;
    std::string previousHash;
    std::string data;

    time_t timestamp;
};


#endif //BLOCKCHAIN_BLOCK_H