#include "Blockchain.h"

Blockchain::Blockchain(int difficulty, int blockGenerationInterval, int adjustmentInterval) {
    this->difficulty = difficulty;
    this->blockGenerationInterval= blockGenerationInterval;
    this->adjustmentInterval = adjustmentInterval;
}

Block Blockchain::createGenesisBlock() {
    return {0,"0",std::time(NULL),"Genesis block",this->difficulty,0};
}

Block Blockchain::getLatestBlock() {
    return this->chain.back();
}

int Blockchain::calculateDifficulty() {
    const Block latestBlock = this->getLatestBlock();

    if (this->chain.size() < this->adjustmentInterval) {
        return this->difficulty;
    }

    const time_t timeTaken = latestBlock.timestamp - this->chain[this->chain.size() - this->adjustmentInterval].timestamp;
    const time_t timeExpected = this->blockGenerationInterval * this->adjustmentInterval * 100;

    if (timeTaken < timeExpected / 2) {
        return this->difficulty += 1;
    }
    if (timeTaken > timeExpected * 2 && this->difficulty > 0){
        return this->difficulty -= 1;
    }
    return this->difficulty;
}
bool Blockchain::addBloc(const Block& newBlock) {
    if (isValidNewBlock(newBlock,this->getLatestBlock())) {
        this->chain.push_back(newBlock);
        return true;
    }
    return false;
}
bool Blockchain::isValidNewBlock(const Block& oldBlock, const Block &newBlock) {
    if (newBlock.index != oldBlock.index + 1) return false;
    if (newBlock.previousHash != oldBlock.hash) return false;
    if (newBlock.timestamp > time(nullptr) + 60 * 1000 ) return false; //validacija casovne znacke za trenutni cas(60sek)
    if (newBlock.timestamp < oldBlock.timestamp - 60 * 1000) return false; //validacija casovne znacke glede na prejsnji blok
    return true;
}