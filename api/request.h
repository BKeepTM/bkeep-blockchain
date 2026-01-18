//
// Created by nejcsorgo on 1/10/2026.
//

#ifndef BLOCKCHAIN_REQUEST_H
#define BLOCKCHAIN_REQUEST_H
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "../Blockchain/Blockchain.h"



class request {
    public:
        static std::string API_URL;
        static int login(std::string username, std::string password);
        static Blockchain getBlockchain();
        static int remove(int index);
        static int post_block(Block block);
        static Block getBlock();
        static std::string getBlockData();
        static CURL *handle;
        static std::string token;

};


#endif //BLOCKCHAIN_REQUEST_H
