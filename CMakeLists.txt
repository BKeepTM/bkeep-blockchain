cmake_minimum_required(VERSION 3.25.2)
project(blockchain)

set(CMAKE_CXX_STANDARD 20)

find_package(MPI COMPONENTS CXX REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(OpenMP REQUIRED)
find_package(CURL REQUIRED )
find_package(nlohmann_json 3.10.5 REQUIRED)
add_definitions(-DOMPI_SKIP_MPICXX)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(blockchain main.cpp
        Blockchain/Blockchain.cpp
        Blockchain/Blockchain.h
        Blockchain/Block.cpp
        Blockchain/Block.h
        sha/sha256.cpp
        sha/sha256.h
        mine/mine.cpp
        mine/mine.h
        util/split_util.cpp
        util/split_util.h
        api/request.cpp
        api/request.h
)

target_include_directories(${PROJECT_NAME} PRIVATE)
#cmake finte...
target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        ${MPI_CXX_LIBRARIES}
        ${OPENSSL_CRYPTO_LIBRARIES}
        ${OPENSSL_SSL_LIBRARY}
        ${OPENMP_CXX_LIBRARY}
        ${CURL_LIBRARIES}
        OpenMP::OpenMP_CXX
        nlohmann_json::nlohmann_json
)