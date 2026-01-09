//
// Created by nejcsorgo on 1/9/2026.
//

#ifndef BLOCKCHAIN_SPLIT_H
#define BLOCKCHAIN_SPLIT_H
#include <sstream>
#include <string>
#include <vector>


class split_util {
public:
    template <typename Out>
     static void split(const std::string &s, char delim, Out result) {
        std::istringstream iss(s);
        std::string item;
        while (std::getline(iss, item, delim)) {
            *result++ = item;
        }
    }

    static std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }
};


#endif //BLOCKCHAIN_SPLIT_H