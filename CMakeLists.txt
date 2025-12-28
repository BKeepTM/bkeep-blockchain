cmake_minimum_required(VERSION 3.28.3)
project(blockchain)

set(CMAKE_CXX_STANDARD 20)

add_executable(blockchain main.cpp
        Blockchain/Blockchain.cpp
        Blockchain/Blockchain.h
        Blockchain/Block.cpp
        Blockchain/Block.h
        sha/sha256.cpp
)