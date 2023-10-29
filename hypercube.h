#include <vector>
#include <unordered_map>
#include <string>
#include "Metric.h"

class Hypercube {
private:
    int numOfHashFunctions;
    int dimension;
    int max_number_M_hypercube;
    int numberOfProbes;
    int number_of_hypercube_dimensions;
    std::vector<Vector> randomVectors;
    std::unordered_map<std::string, std::vector<Vector>> hypercubeCells;
    Metric* metric;

    void generateRandomVectors();
    std::string computeBinaryCode(const Vector& point) const;
    std::vector<std::string> getNeighbors(const std::string& code, int distance) const;
    Vector findClosestVector(const Vector& point, const std::vector<Vector>& candidates) const;

public:
    Hypercube(int functions, Metric* metricInstance, int max_number_M_hypercube, int dim);
    void build(const std::vector<Vector>& dataset);
    Vector nearestNeighbor(const Vector& point) const;
    std::vector<Vector> NClosestVectors(const Vector& point, int N) const;
    std::vector<Vector> rangeSearch(const Vector& point, double range) const;
};
