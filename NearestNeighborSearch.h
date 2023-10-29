#pragma once
#include <vector>
#include <cstdint>

using Vector = std::vector<uint8_t>;

class NearestNeighborSearch {
public:
    virtual void insert(const Vector& image) = 0;
    virtual Vector query(const Vector& image) = 0;
    virtual Vector reverseQuery(const Vector& image) = 0;
    virtual ~NearestNeighborSearch() {} // Virtual destructor for proper cleanup
};
