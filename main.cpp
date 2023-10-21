#include "dataset.h"
#include <iostream>

int main() {
    Dataset dataset;
    try {
        dataset.readFromFile("../input.dat");
        std::cout << "Successfully read dataset with " << dataset.numOfImages << " images." << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}
