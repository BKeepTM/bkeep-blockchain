//
// Created by nejcsorgo on 12/28/2025.
//
#include "mine.h"

Block mine(const Block &previousBlock, std::string data, int difficulty) {
    int token = 0;
    std::string hash;
    time_t timestamp = time(0);
    int index = previousBlock.index + 1;
    volatile bool flag=false;
    // del kode ko je treba pararelizirati
    #pragma omp parallel shared(flag) // xD
    {
        do {
            token++;
            hash = sha::sha256(
                std::to_string(index) + data + std::to_string(timestamp) + previousBlock.hash + std::to_string(difficulty) +
                std::to_string(token));
        } while (hash.substr(0, difficulty) != std::string(difficulty, '0') && !flag);
        flag = true;
    }
    Block newBlock(
        index,
        previousBlock.hash,
        timestamp,
        data,
        difficulty,
        token,
        hash
    );
};