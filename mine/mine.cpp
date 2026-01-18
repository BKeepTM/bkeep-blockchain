#include "mine.h"

#include <omp.h>


Block mine(const Block &previousBlock, std::string data, int in_difficulty, long unsigned token) {
    int lmt = 10000000;
    long unsigned token_lmt = token + lmt;
    std::string hash;
    const auto p1 = std::chrono::system_clock::now();
    time_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(
                   p1.time_since_epoch()).count();
    int index = previousBlock.index + 1;
    std::string prev_hash = previousBlock.hash;
    volatile bool flag=false;

    int difficulty = in_difficulty;
    std::string r_hash = "-1";

    std::string zero = std::string(difficulty, '0');
    std::string timestamp_str = std::to_string(timestamp);
    std::string index_str = std::to_string(index);
    std::string difficulty_str = std::to_string(difficulty);

    std::string constant =  index_str + data + timestamp_str  + prev_hash + difficulty_str;
    #pragma omp parallel default(none) shared(flag,r_hash,token) firstprivate(hash,lmt,constant,zero,difficulty)
    {
        int inv_limit = lmt/omp_get_num_threads();
        long unsigned local = token + inv_limit * omp_get_thread_num();;
        long unsigned local_limit = local + inv_limit;
        sha sha;
        do {
            local++;
            hash = sha.sha256(constant  + std::to_string(local));

        } while (hash.substr(0, difficulty) !=  zero  && local < local_limit && !flag);


        if (hash.substr(0, difficulty) == std::string(difficulty, '0')){
            #pragma omp critical
            {
                flag = true;
                r_hash = hash;
                token = local;
            }
        }   ;
    }
    if (!flag){
        index = -1;
        r_hash = -1;
        token = token_lmt;
        timestamp = 0;
    } else {
        //std::cout<<"Mined hash with diff"<<difficulty<<r_hash<<std::endl;
        std::cout.flush();
        Block newBlock(
            index,
            previousBlock.hash,
            timestamp,
            data,
            in_difficulty,
            token,
            r_hash
    );
        std::cout<<"Mined block data: "<<newBlock.toString()<<std::endl;
        return newBlock;

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