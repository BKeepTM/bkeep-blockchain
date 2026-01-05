//
// Created by nejcsorgo on 12/30/2025.
//

#ifndef BLOCKCHAIN_MINE_H
#define BLOCKCHAIN_MINE_H
#include "../sha/sha256.h"
#include "../Blockchain/Block.h"
#include <string>
#include <mpi.h>


Block mine(const Block &previousBlock, std::string data, int difficulty);

#endif //BLOCKCHAIN_MINE_H