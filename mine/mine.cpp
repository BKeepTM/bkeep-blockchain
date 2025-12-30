//
// Created by nejcsorgo on 12/28/2025.
//
#include "mine.h"

void mine(const Block &previousBlock, std::string data, int difficulty) {
    int token = 0;
    std::string hash;
    time_t timestamp = time(0);
    int index = previousBlock.index + 1;
    do {
        token++;
        hash = sha::sha256(
            std::to_string(index) + previousBlock.hash + std::to_string(timestamp) + data + std::to_string(difficulty) +
            std::to_string(token));
    } while (hash.substr(0, difficulty) != std::string(difficulty, '0'));
    Block newBlock(
        index,
        previousBlock.hash,
        timestamp,
        data,
        difficulty,
        token,
        hash
    );
}
