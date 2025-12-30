#include "Block.h"



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