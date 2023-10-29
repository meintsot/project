#include "Metric.h"
#include <vector>
#include <unordered_map>
#include <random>

class Hypercube {
private:
    int numOfHashFunctions;
    std::vector<Vector> randomVectors;  // Random vectors for projections
    std::unordered_map<std::string, std::vector<Vector>> hypercubeCells;
    Metric* metric;

    void generateRandomVectors();
    std::string computeBinaryCode(const Vector& point) const;

public:
    Hypercube(int functions, Metric* metricInstance);
    void build(const std::vector<Vector>& dataset);
    Vector nearestNeighbor(const Vector& point) const;
    std::vector<Vector> NClosestVectors(const Vector& point, int N) const;
    std::vector<Vector> rangeSearch(const Vector& point, double range) const;
};
