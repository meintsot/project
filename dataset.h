#pragma once

#include <cstdint>
#include <vector>
#include <fstream>

class Dataset {
public:
    uint32_t magicNumber;
    uint32_t numOfImages;
    uint32_t numRows;
    uint32_t numCols;
    std::vector<std::vector<uint8_t>> images;

    // Constructor
    Dataset();

    // Methods to read and process the dataset
    void readFromFile(const std::string& filename);
};
