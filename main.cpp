#include <iostream>
#include <mpi.h>
#include <random>
#include <thread>
#include <curl/curl.h>
#include "util/split_util.h"
#include "Blockchain/Block.h"
#include "Blockchain/Blockchain.h"
#include "mine/mine.h"

#define DEBUG 0

int main(int argc, char** argv) {
    std::string bchain_str;
    //difficulty ne sme bit premali na zacetku, sicer
    Blockchain chain = Blockchain(4,4,2);
    chain.createGenesisBlock();
    int process_Rank, size_Of_Cluster;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,255); // distribution in range [1, 6]
    int random = dist6(rng);
    char xd = random;
    std::string data = std::to_string(xd);
    int prev_token = 0;
    MPI_Status status;
    MPI_Request request;
    while (1) {
        int probe_flag = false;
        MPI_Iprobe(MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&probe_flag,&status);
        int difficulty = chain.difficulty;
        if (DEBUG) {
            std::cout<<"Process: "<<process_Rank<<" is mining. last token: "<<prev_token<<" difficulty: "<<difficulty<<std::endl;
            std::cout.flush();
        }
        if (probe_flag == true) {
            int count;
            MPI_Get_count(&status,MPI_CHAR,&count);
          std::vector<char> char_vec(count);
            if (DEBUG) {
                std::cout<<"Process: "<<process_Rank<<" received chain. Size received: "<<count<<std::endl;
                std::cout.flush();
            }
            MPI_Get_count(&status,MPI_CHAR,&count);
            MPI_Recv(char_vec.data(), count, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,&status);
            std::string recv_chain_str(char_vec.begin(),char_vec.end());
            if (DEBUG) {
                std::cout<<"Process: "<<process_Rank<<" received chain. Chain: "<<recv_chain_str<<std::endl;
                std::cout.flush();
            }
            Blockchain recv_chain= Blockchain::fromString(recv_chain_str);
            int compare = Blockchain::checkDifficulty(recv_chain,chain);
            if (compare == 1) {
                std::cout<<"received better chain: "<<process_Rank<<"from: "<<status.MPI_SOURCE<<std::endl;
                std::cout.flush();
                recv_chain.difficulty = chain.difficulty;
                recv_chain.adjustmentInterval = chain.adjustmentInterval;
                recv_chain.blockGenerationInterval = chain.adjustmentInterval;
                chain.chain = recv_chain.chain;
            } else if (compare == 0)  {

                std::cout<<"received worse chain: "<<process_Rank<<"from: "<<status.MPI_SOURCE<<"mine:  "<<chain.calculateCumulativeDifficulty()<<"theirs:  "<<recv_chain.calculateCumulativeDifficulty()<<std::endl;
                std::cout.flush();
                //std::string chain_str = chain.toString();
                //MPI_Send(chain_str.c_str(),chain_str.size(),MPI_CHAR,status.MPI_SOURCE,0,MPI_COMM_WORLD);
            } else {
                if (DEBUG) {
                    std::cout<<"received same chain: "<<process_Rank<<"from: "<<status.MPI_SOURCE<<std::endl;
                    std::cout.flush();
                }
            }
            //return 0;
            continue;
        }

        auto block = mine(chain.getLatestBlock(),data,difficulty,prev_token);
        if (block.index == -1) {
            prev_token = block.token;
            continue;
        }
        if (!chain.addBloc(block)) {
            if (DEBUG) {
                std::cout<<"Mined invalid block."<<std::endl;
                std::cout.flush();
            }
            continue;
        }
        chain.difficulty = chain.calculateDifficulty();
        prev_token = 0;
        bchain_str = chain.toString();
        //std::string bchain_str = std::string();
        if (DEBUG) {
            std::cout<<"Process: "<<process_Rank<<" mined block: "<<block.toString()<<std::endl<<"Blockchain: "<<chain.toString()<<std::endl;
            std::cout.flush();
        }
        for (int i = 0; i < size_Of_Cluster; i++) {
            if (i != process_Rank) {
                std::cout<<"Process: "<<process_Rank<<" Sending size: "<<bchain_str.size()<<" "<<bchain_str<<" to:"<<i<<std::endl;
                std::cout.flush();
                MPI_Send(bchain_str.c_str(),bchain_str.size(),MPI_CHAR,i,0,MPI_COMM_WORLD);
            }
        }
        sleep(1);
    }

    printf("Hello World from process %d of %d\n", process_Rank, size_Of_Cluster);

    MPI_Finalize();
    return 0;
}