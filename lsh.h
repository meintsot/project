// LSH.h
#include "Metric.h"
#include <vector>
#include <unordered_map>

class LSH {
private:
    int numOfHashTables;
    int numOfHashFunctions;
    int numberOfProbes;
    int dimension;
    std::vector<Vector> randomVectors;
    std::vector<std::unordered_map<int, std::vector<Vector>>> hashTables;
    Metric* metric;

    void generateRandomVectors();
    int computeHashValue(const Vector& point, int tableIndex) const;
    std::vector<Vector> retrieveCandidates(const Vector& point) const;

public:
    LSH(int tables, int functions, Metric* metricInstance, int numberOfProbes, int dim);
    void build(const std::vector<Vector>& dataset);
    Vector nearestNeighbor(const Vector& point) const;
    std::vector<Vector> NClosestVectors(const Vector& point, int N) const;
    std::vector<Vector> rangeSearch(const Vector& point, double range) const;
};
