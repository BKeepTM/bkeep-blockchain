//
// Created by nejcsorgo on 12/28/2025.
//
#include "mine.h"




Block mine(const Block &previousBlock, std::string data, int in_difficulty, int token) {

    int token_lmt = token + 10000;
    std::string hash;
    const auto p1 = std::chrono::system_clock::now();
    time_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(
                   p1.time_since_epoch()).count();
    int index = previousBlock.index + 1;
    std::string prev_hash = previousBlock.hash;
    volatile bool flag=false;
    int r_token;
    int difficulty = in_difficulty;
    std::string r_hash = "-1";
    // del kode ko je treba pararelizirati
    #pragma omp parallel default(none) shared(flag,token,r_hash) firstprivate(hash,timestamp,index,prev_hash,token_lmt,data,difficulty)  // xD
    {
        int local;
        sha sha;
        std::string timestamp_str = std::to_string(timestamp);
        std::string index_str = std::to_string(index);
        std::string difficulty_str = std::to_string(difficulty);
        std::string constant =  index_str + data + timestamp_str  + prev_hash + difficulty_str;
        do {
            #pragma omp critical
            local = ++token;
            hash = sha.sha256(constant  + std::to_string(local));

        } while (hash.substr(0, difficulty) != std::string(difficulty, '0') && !flag && token < token_lmt);

        #pragma omp critical
        {
            if (hash.substr(0, difficulty) != std::string(difficulty, '0')){
                //index = -1;
                //r_hash = "-1";
            } else {
                flag = true;
                r_hash = hash;
                token = local;
            }
        };
    }
    if (!flag ){
        index = -1;
        r_hash = -1;
        //token = -1;
        timestamp = 0;
    }
    Block newBlock(
            index,
            previousBlock.hash,
            timestamp,
            data,
            in_difficulty,
            token,
            r_hash
    );
    return newBlock;
};