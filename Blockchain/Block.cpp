#include "Block.h"

#define DEBUG 1
Block::Block(int index, std::string previousHash, time_t timestamp, std::string data, int difficulty, int token) {
    this->index = index;
    this->previousHash = std::move(previousHash);
    this->timestamp= timestamp;
    this->data= std::move(data);
    this->difficulty=difficulty;
    this->token=token;
    this->hash=calculateHash();
}
Block::Block(int index, std::string previousHash,time_t timestamp, std::string data, int difficulty, int token, std::string &hash){
    this->index = index;
    this->previousHash = std::move(previousHash);
    this->timestamp= timestamp;
    this->data= std::move(data);
    this->difficulty=difficulty;
    this->token=token;
    this->hash=hash;// xD
}
std::string Block::calculateHash() {
    return sha::sha256(std::to_string(this->index) + this->previousHash + this->data + std::to_string(this->timestamp) + std::to_string(this->difficulty) + std::to_string(this->token));
}
std::string Block::toString() {
    return std::to_string(this->index) + "" +";" + this->previousHash + ";" + std::to_string(this->timestamp) + ";" +  this->data+ ";" + std::to_string(this->difficulty) + ";" + std::to_string(this->token) + ";" + hash;
}

Block Block::fromString(std::string input) {
    if (DEBUG) {
        std::cout<<"Input string in block From String: "<<input<<std::endl;
        std::cout.flush();
    }

    std::vector<std::string> block_string = split_util::split(input,';');
    return {stoi(block_string[0]),block_string[1],time_t(stol(block_string[2])),block_string[3],stoi(block_string[4]),stoi(block_string[5]),block_string[6]};
}
