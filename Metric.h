#pragma once
#include <vector>
#include <cstdint>

using Vector = std::vector<uint8_t>;

class Metric {
public:
    // Pure virtual function for distance calculation
    virtual double calculate(const std::vector<uint8_t>& vec1, const std::vector<uint8_t>& vec2) const = 0;

    virtual ~Metric() {} // Virtual destructor for proper cleanup
};
