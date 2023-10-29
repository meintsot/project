#include <algorithm>
#include "KMeans.h"

// Constructor
KMeans::KMeans(int k, int dimension) : k(k), dimension(dimension), lsh(dimension, k), hypercube(dimension, k) {}

// k-Means++ Initialization
void KMeans::initializeCentroids(const std::vector<std::vector<double>>& data) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, data.size() - 1);

    // Choose one data point randomly as the first centroid
    centroids.push_back(data[distrib(gen)]);

    // Choose remaining centroids
    for (int i = 1; i < k; ++i) {
        std::vector<double> minDistances(data.size(), std::numeric_limits<double>::max());

        // Calculate squared distances from points to nearest centroid
        for (size_t j = 0; j < data.size(); ++j) {
            for (const auto& centroid : centroids) {
                double dist = computeDistance(data[j], centroid);
                minDistances[j] = std::min(minDistances[j], dist * dist);
            }
        }

        // Choose next centroid with probability proportional to squared distance
        std::discrete_distribution<> weightedDistrib(minDistances.begin(), minDistances.end());
        centroids.push_back(data[weightedDistrib(gen)]);
    }
}

// MacQueen's k-Means Updating
void KMeans::updateCentroidsMacQueen(const std::vector<std::vector<double>>& data) {
    // Initialize clusters and counts
    std::vector<std::vector<std::vector<double>>> clusters(k);
    std::vector<int> clusterCounts(k, 0);

    for (const auto& point : data) {
        // Find nearest centroid
        int nearestCentroidIdx = std::distance(
                centroids.begin(),
                std::min_element(
                        centroids.begin(),centroids.end(),
                        [&](const std::vector<double>& a, const std::vector<double>& b
                        ) {
                            return computeDistance(point, a) < computeDistance(point, b);
                        }));

        // Add point to cluster and update centroid incrementally
        clusters[nearestCentroidIdx].push_back(point);
        clusterCounts[nearestCentroidIdx]++;
        int n = clusterCounts[nearestCentroidIdx];
        for (size_t j = 0; j < dimension; ++j) {
            centroids[nearestCentroidIdx][j] = (centroids[nearestCentroidIdx][j] * (n - 1) + point[j]) / n;
        }
    }
}

// L2 distance computation
double KMeans::computeDistance(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    double sum = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        double diff = vec1[i] - vec2[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}
