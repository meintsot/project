#pragma once
#include <vector>
#include <cstdint>

using Vector = std::vector<uint8_t>;

class Metric {
public:
    // Pure virtual function for distance calculation
    virtual double calculate(const Vector& vec1, const Vector& vec2) const = 0;

    virtual ~Metric() {} // Virtual destructor for proper cleanup
};
