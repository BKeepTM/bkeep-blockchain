#ifndef BLOCKCHAIN_BLOCKCHAIN_H
#define BLOCKCHAIN_BLOCKCHAIN_H
#include <vector>
#include <ctime>
#include <chrono>
#include "Block.h"


class Blockchain {
public:
    Blockchain(int difficulty, int blockGenerationInterval,int adjustmentInterval);
    void createGenesisBlock();
    Block getLatestBlock();
    int calculateDifficulty();
    int calculateCumulativeDifficulty();
    static bool isValidNewBlock(const Block& oldBlock, const Block &newBlock);
    // vrne true/false glede na to, ce je prvi chain bolsi al slabsi
    static int checkDifficulty(Blockchain chain1,Blockchain chain2);
    bool addBloc(const Block& newBlock);

    std::string toString();
    static Blockchain fromString(std::string input);
    std::vector<Block> chain;

    int difficulty;
    int blockGenerationInterval;
    int adjustmentInterval;

    bool isMining;
};


#endif //BLOCKCHAIN_BLOCKCHAIN_H