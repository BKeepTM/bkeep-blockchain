//
// Created by nejcsorgo on 12/28/2025.
//
#include "mine.h"




Block mine(const Block &previousBlock, std::string data, int difficulty, int token) {
    int token_lmt = token + 10000;
    std::string hash;
    const auto p1 = std::chrono::system_clock::now();
    time_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(
                   p1.time_since_epoch()).count();
    int index = previousBlock.index + 1;
    volatile bool flag=false;
    // del kode ko je treba pararelizirati
    #pragma omp parallel shared(flag,token) // xD
    {
        do {
            #pragma omp critical
            token++;
            hash = sha::sha256(
                std::to_string(index) + data + std::to_string(timestamp) + previousBlock.hash + std::to_string(difficulty) +
                std::to_string(token));
        } while (hash.substr(0, difficulty) != std::string(difficulty, '0') && !flag && token < token_lmt);
        #pragma omp critical
        flag = true;
    }
    if (hash.substr(0, difficulty) != std::string(difficulty, '0'))
        index = -1;
    Block newBlock(
        index,
        previousBlock.hash,
        timestamp,
        data,
        difficulty,
        token,
        hash
    );
    return newBlock;
};