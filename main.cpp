#include "dataset.h"
#include <iostream>
#include <map>
#include <string>

std::map<std::string, std::string> parseArguments(int argc, char* argv[]) {
    std::map<std::string, std::string> arguments;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg.size() > 1 && arg[0] == '-') {
            if (i + 1 < argc) {
                arguments[arg] = argv[i + 1];
                i++;
            } else {
                arguments[arg] = "";
            }
        }
    }

    return arguments;
}

std::map<std::string, std::string> readConfigFile(const std::string& filename) {
    std::ifstream inFile(filename);
    std::map<std::string, std::string> config;
    if (inFile.is_open()) {
        std::string line;
        while (getline(inFile, line)) {
            size_t delimiterPos = line.find(":");
            if (delimiterPos != std::string::npos) {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);
                config[key] = value;
            }
        }
        inFile.close();
    } else {
        throw std::runtime_error("Unable to open configuration file: " + filename);
    }

    return config;
}

int main(int argc, char* argv[]) {
    Dataset dataset;

    // Parse command line arguments
    std::map<std::string, std::string> arguments = parseArguments(argc, argv);

    // If a configuration file is provided, read it
    if (arguments.find("-c") != arguments.end()) {
        std::string configFile = arguments["-c"];
        std::map<std::string, std::string> config = readConfigFile(configFile);
        // Now, you can access configuration values via the 'config' map
        // Example: config["number_of_clusters"]
    }

    // If an input file is provided, read the dataset
    if (arguments.find("-i") != arguments.end()) {
        std::string inputFile = arguments["-i"];
        try {
            dataset.readFromFile(inputFile);
            std::cout << "Successfully read dataset with " << dataset.numOfImages << " images." << std::endl;
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
    }

    return 0;
}
