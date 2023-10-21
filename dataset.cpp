#include "dataset.h"
#include <fstream>
#include <stdexcept>

uint32_t swapEndian(uint32_t val) {
    return ((val << 24) & 0xFF000000) |
           ((val << 8)  & 0x00FF0000) |
           ((val >> 8)  & 0x0000FF00) |
           ((val >> 24) & 0x000000FF);
}

Dataset::Dataset() : magicNumber(0), numOfImages(0), numRows(0), numCols(0) {}

void Dataset::readFromFile(const std::string& filename) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    // Read the header
    infile.read((char*)&magicNumber, sizeof(uint32_t));
    magicNumber = swapEndian(magicNumber);

    infile.read((char*)&numOfImages, sizeof(uint32_t));
    numOfImages = swapEndian(numOfImages);

    infile.read((char*)&numRows, sizeof(uint32_t));
    numRows = swapEndian(numRows);

    infile.read((char*)&numCols, sizeof(uint32_t));
    numCols = swapEndian(numCols);

    // Read images
    images.resize(numOfImages, std::vector<uint8_t>(numRows * numCols));
    for (uint32_t i = 0; i < numOfImages; ++i) {
        infile.read((char*)images[i].data(), numRows * numCols);
    }

    infile.close();
}
