#include "Blockchain.h"



Blockchain::Blockchain(int difficulty, int blockGenerationInterval, int adjustmentInterval) {
    this->difficulty = difficulty;
    this->blockGenerationInterval= blockGenerationInterval;
    this->adjustmentInterval = adjustmentInterval;
}
// same ce je prazno
void Blockchain::createGenesisBlock() {
    const auto p1 = std::chrono::system_clock::now();
    time_t timestamp =std::chrono::duration_cast<std::chrono::seconds>(
                   p1.time_since_epoch()).count();
    if (chain.empty())
        chain.emplace_back(0,"0",timestamp,"Genesis block",this->difficulty,0);
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
    const time_t timeExpected = this->blockGenerationInterval * this->adjustmentInterval * 0.5;

    if (timeTaken < timeExpected / 2) {
        return this->difficulty + 1;
    }
    if (timeTaken > timeExpected * 2 && this->difficulty > 0){
        return this->difficulty - 1;
    }
    return this->difficulty;
}

int Blockchain::calculateCumulativeDifficulty() {
    int cumulativeDifficulty = 0;
    for (int i = 1; i < chain.size(); i++) {
        if (!this->isValidNewBlock(chain[i-1],chain[i])) {
            return -1;
        }
        cumulativeDifficulty += chain[i].difficulty*chain[i].difficulty;
    }
    return cumulativeDifficulty;
}

int Blockchain::checkDifficulty(Blockchain chain1, Blockchain chain2)
{
    int diff1= chain1.calculateCumulativeDifficulty();
    int diff2 = chain2.calculateCumulativeDifficulty();
    if (diff1 == diff2)
        return 2;
   return diff1 > diff2;

}
bool Blockchain::addBloc(const Block& newBlock) {
    if (isValidNewBlock(this->getLatestBlock(),newBlock)) {
        this->chain.push_back(newBlock);
        this->difficulty = this->calculateDifficulty();
        return true;
    }
    return false;
}

std::string Blockchain::toString() {
    std::string returnStr;
    for (int i = 0; i < chain.size();i++) {
        returnStr+=chain[i].toString();
        if (i +1 != chain.size()) {
            returnStr+=",";
        }
    }
    return returnStr;
}
Blockchain Blockchain::fromString(std::string input) {
    std::vector<std::string> block_string_vct = split_util::split(input,',');
    std::vector<Block> chain;
    for (int i = 0; i <block_string_vct.size(); i++) {
        chain.push_back(Block::fromString(block_string_vct[i]));
    }
    Blockchain out_chain(5,5,4);
    out_chain.chain = chain;
    return out_chain;
};

bool Blockchain::isValidNewBlock(const Block& oldBlock, const Block &newBlock) {\
    const auto p1 = std::chrono::system_clock::now();
    time_t timestamp =std::chrono::duration_cast<std::chrono::seconds>(
                   p1.time_since_epoch()).count();

    if (newBlock.index != oldBlock.index + 1) return false;
    if (newBlock.previousHash != oldBlock.hash) return false;
    if (newBlock.timestamp > oldBlock.timestamp + 60 * 1000) return false; //validacija casovne znacke za trenutni cas(60sek)
    if (newBlock.timestamp < oldBlock.timestamp - 60 * 1000 ) return false; //validacija casovne znacke glede na prejsnji blok
    //if (newBlock.previousHash != oldBlock.calculateHash()) return false;
    return true;
}