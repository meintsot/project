#ifndef HYBRIDKMEANS_H
#define HYBRIDKMEANS_H

#include "KMeansPlusPlusInitializer.h"
#include "LSH.h"
#include "Hypercube.h"
#include "LloydsMacQueen.h"
#include "Metric.h"
#include <vector>
#include <unordered_map>

class HybridKMeans {
private:
    int k;
    std::vector<Vector> centroids;
    std::vector<int> labels;
    LSH* lsh;
    Hypercube* hypercube;
    LloydsMacQueen* lloydsMacQueen;
    Metric* metric;
    bool useLSH;
    bool useHypercube;
    bool useLloydsMacQueen;
    int tables;
    int functions;
    int dimension;
    int number_of_hypercube_dimensions;
    int max_number_M_hypercube;
    int number_of_probes;

public:
    HybridKMeans(int numCentroids, Metric* metricInstance, int numOfHashTables, int numOfHashFunctions, int number_of_hypercube_dimensions, int max_number_M_hypercube, int number_of_probes, bool useLSHFlag, bool useHypercubeFlag, bool useLloydsMacQueenFlag, int dimension);
    ~HybridKMeans();
    void fit(const std::vector<Vector>& dataset);
    std::vector<double> calculateSilhouette(const std::vector<Vector>& dataset);
};

#endif
