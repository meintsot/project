#include "LSH.h"
#include "Hypercube.h"
#include <vector>
#include <unordered_set>

class KMeans {
private:
    int k;  // Number of clusters
    int dimension;
    std::vector<std::vector<double>> centroids;
    LSH lsh;
    Hypercube hypercube;

    void initializeCentroids(const std::vector<std::vector<double>>& data);
    void updateCentroidsMacQueen(const std::vector<std::vector<double>>& data);
    double computeDistance(const std::vector<double>& vec1, const std::vector<double>& vec2);  // L2 distance

public:
    KMeans(int k, int dimension);

    // Convergence methods
    void convergeLloyds(const std::vector<std::vector<double>>& data);
    void convergeLSH(const std::vector<std::vector<double>>& data);
    void convergeHypercube(const std::vector<std::vector<double>>& data);

    // Getter for centroids
    std::vector<std::vector<double>> getCentroids() const;
};
