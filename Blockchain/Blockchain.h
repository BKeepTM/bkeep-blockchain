#ifndef BLOCKCHAIN_BLOCKCHAIN_H
#define BLOCKCHAIN_BLOCKCHAIN_H
#include <vector>
#include <ctime>
#include "Block.h"


class Blockchain {
public:
    Blockchain(int difficulty, int blockGenerationInterval,int adjustmentInterval);
    Block createGenesisBlock();
    Block getLatestBlock();
    int calculateDifficulty();
    static bool isValidNewBlock(const Block& oldBlock, const Block &newBlock);
    bool addBloc(const Block& newBlock);

    std::vector<Block> chain;

    int difficulty;
    int blockGenerationInterval;
    int adjustmentInterval;

    bool isMining;
};


#endif //BLOCKCHAIN_BLOCKCHAIN_H