#ifndef BLOCKCHAIN_BLOCK_H
#define BLOCKCHAIN_BLOCK_H
#include <cstdint>
#include <string>
#include <utility>
#include "../util/split_util.h"
#include <vector>
#include "../sha/sha256.h"


class Block {
    public:
    Block(int index, std::string previousHash,time_t timestamp, std::string data, int difficulty, long unsigned token);
    Block(int index, std::string previousHash,time_t timestamp, std::string data, int difficulty, long unsigned token, std::string &hash);
    std::string calculateHash();
    std::string toString();
    static Block fromString(std::string input);

    int index;
    int difficulty;
    long unsigned token;

    std::string hash;
    std::string previousHash;
    std::string data;

    time_t timestamp;
};


#endif //BLOCKCHAIN_BLOCK_H