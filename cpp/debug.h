#ifndef DOCX_CHAR_CNV_DEBUG_H
#define DOCX_CHAR_CNV_DEBUG_H

#include "common.h"

namespace debug {
    void printVecStruct(std::vector<TokenizedText> &);

    void printVecStruct(std::vector<TokenizedText> &vecStruct) {
        std::cout << "[";
        for (const auto &stru : vecStruct) {
            std::cout << "{token:" << stru.token << ", text:" << stru.text << "}, ";
        }
        std::cout << "]" << std::endl;
    }

    void printVecVecSize(std::vector<std::vector<size_t>> &vecVecSize) {
        std::cout << "[ ";
        for (const std::vector<size_t> &vec0 : vecVecSize) {
            std::cout << "[ ";
            for (const size_t &n : vec0) {
                std::cout << n;
                std::cout << ", ";
            }
            std::cout << "]";
        }
        std::cout << " ]" << std::endl;
    }
}


#endif //DOCX_CHAR_CNV_DEBUG_H
