#include <iostream>
#include <mpi.h>
#include <random>
#include <thread>

#include "util/split_util.h"
#include "Blockchain/Block.h"
#include "Blockchain/Blockchain.h"
#include "mine/mine.h"

#define DEBUG 1

int main(int argc, char** argv) {
    Blockchain chain = Blockchain(0,200000,2);
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
    volatile bool received = false;
    int difficulty = 0;
    int probe_flag;
    int status_flag;
    MPI_Status status,count_status;
    MPI_Request request;
    bool bcast = false;
    while (1) {
        MPI_Iprobe(0,0,MPI_COMM_WORLD,&probe_flag,&status);
        if (DEBUG) {
            std::cout<<"Process: "<<process_Rank<<" is mining."<<std::endl;
            std::cout.flush();
        }
        if (probe_flag == true) {
            if (DEBUG) {
                std::cout<<"Process: "<<process_Rank<<" received chain. Size received: "<<std::endl;
                std::cout.flush();
            }
            std::vector<char> char_vec;
            int count;
            MPI_Get_count(&count_status,MPI_CHAR,&count);
            MPI_Recv(static_cast<void*>(&char_vec), count, MPI_CHAR, 0, 0, MPI_COMM_WORLD,&status);
            std::string recv_chain_str;
            recv_chain_str.assign(char_vec.begin(), char_vec.end());
            Blockchain recv_chain= Blockchain::fromString(recv_chain_str);
            int compare = Blockchain::checkDifficulty(recv_chain,chain);
            if (compare == 1) {
                std::cout<<"received better chain: "<<process_Rank<<"from: "<<status.MPI_SOURCE<<std::endl;
                std::cout.flush();
                recv_chain.difficulty = chain.difficulty;
                recv_chain.adjustmentInterval = chain.adjustmentInterval;
                recv_chain.blockGenerationInterval = chain.adjustmentInterval;
                chain = recv_chain;
            } else if (compare == 0)  {
                std::cout<<"received worse chain: "<<process_Rank<<"from: "<<status.MPI_SOURCE<<std::endl;
                std::cout.flush();
                std::string chain_str = chain.toString();
                MPI_Send(&chain_str,chain_str.size(),MPI_CHAR,status.MPI_SOURCE,0,MPI_COMM_WORLD);
            }
            //return 0;
            continue;
        }
        if (bcast) {
            if (DEBUG) {
                std::cout<<"Process: "<<process_Rank<<" broadcasting."<<"Blockchain size: "<<chain.chain.size()<<std::endl;
                std::cout.flush();
            }
            std::string bchain_str = chain.toString();
            //broadcast je baje shit :/
            MPI_Bcast(static_cast<void*>(&bchain_str),bchain_str.size(),MPI_CHAR,0,MPI_COMM_WORLD);
            std::this_thread::sleep_for(std::chrono_literals::operator ""s(1)); // 1 s pocaka za reply
            bcast = false;
            continue;
        }
        int difficulty = chain.calculateDifficulty();
        auto block = mine(chain.getLatestBlock(),data,difficulty);
        if (!chain.addBloc(block)) {
            if (DEBUG) {
                std::cout<<"Mined invalid block."<<std::endl;
                std::cout.flush();
            }
        }
        std::string bchain_str = chain.toString();
        if (DEBUG) {
            std::cout<<"Process: "<<process_Rank<<" mined block: "<<block.toString()<<"Blockchain size: "<<chain.chain.size()<<std::endl;
            std::cout.flush();
        }
        bcast = true;
        //MPI_Bcast(&bchain_str,bchain_str.size(),MPI_CHAR,0,MPI_COMM_WORLD);


    }

    printf("Hello World from process %d of %d\n", process_Rank, size_Of_Cluster);

    MPI_Finalize();
    return 0;
}