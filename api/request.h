//
// Created by nejcsorgo on 1/10/2026.
//

#ifndef BLOCKCHAIN_REQUEST_H
#define BLOCKCHAIN_REQUEST_H
#include <string>
#include <curl/curl.h>

#include "../Blockchain/Blockchain.h"



class request {
    public:
        static std::string API_URL;
        static int login(std::string username, std::string password);
        static Blockchain getBlockchain();
        static int remove(int index);
        static int post_block(Block block);
        static Blockchain getBlock();
        static CURL *handle;
        static std::string token;

};
std::string request::API_URL ="http://localhost:3000";
CURL *request::handle = curl_easy_init();

#endif //BLOCKCHAIN_REQUEST_H
