// LSH.h
#include "Metric.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <random>

class LSH {
private:
    int numOfHashTables;
    int numOfHashFunctions;
    std::vector<Vector> randomVectors;  // Random vectors for projections
    std::vector<std::unordered_map<int, std::vector<Vector>>> hashTables;
    Metric* metric;

    void generateRandomVectors();
    int computeHashValue(const Vector& point, int tableIndex) const;

public:
    LSH(int tables, int functions, Metric* metricInstance);
    void build(const std::vector<Vector>& dataset);
    Vector nearestNeighbor(const Vector& point) const;
    std::vector<Vector> NClosestVectors(const Vector& point, int N) const;
    std::vector<Vector> rangeSearch(const Vector& point, double range) const;
};
