#include "LloydsMacQueen.h"
#include <algorithm>
#include <limits>
#include <numeric>
#include <random>

LloydsMacQueen::LloydsMacQueen(int kClusters, Metric* metricInstance)
        : k(kClusters), metric(metricInstance) {}

void LloydsMacQueen::fit(const std::vector<Vector>& dataset) {
    initializeCentroidsKMeansPlusPlus(dataset);
    bool changed = true;

    while (changed) {
        changed = false;
        std::vector<int> assignments(dataset.size(), -1);
        std::vector<int> clusterSizes(k, 0);  // Initialize cluster sizes to zeros

        // MacQueen's Iterative Refinement
        for (size_t i = 0; i < dataset.size(); ++i) {
            int closestCluster = -1;
            double minDist = std::numeric_limits<double>::max();

            for (int j = 0; j < k; ++j) {
                double dist = metric->calculate(dataset[i], centroids[j]);
                if (dist < minDist) {
                    minDist = dist;
                    closestCluster = j;
                }
            }

            // Increment the cluster size
            ++clusterSizes[closestCluster];

            // Update the centroid immediately
            Vector& currentCentroid = centroids[closestCluster];
            const Vector& dataPoint = dataset[i];
            for (size_t dim = 0; dim < dataPoint.size(); ++dim) {
                currentCentroid[dim] = (currentCentroid[dim] * (clusterSizes[closestCluster] - 1) + dataPoint[dim]) / clusterSizes[closestCluster];
            }

            // Check for changes in cluster assignments
            if (assignments[i] != closestCluster) {
                assignments[i] = closestCluster;
                changed = true;
            }
        }
    }
}

Vector LloydsMacQueen::assign(const Vector& point) const {
    Vector nearest;
    double minDist = std::numeric_limits<double>::max();
    for (const auto& centroid : centroids) {
        double dist = metric->calculate(point, centroid);
        if (dist < minDist) {
            minDist = dist;
            nearest = centroid;
        }
    }
    return nearest;
}

const std::vector<Vector>& LloydsMacQueen::getCentroids() const {
    return centroids;
}

void LloydsMacQueen::initializeCentroidsKMeansPlusPlus(const std::vector<Vector>& dataset) {
    centroids.clear();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, dataset.size() - 1);

    // 1. Choose one data point randomly
    centroids.push_back(dataset[dis(gen)]);

    while (centroids.size() < static_cast<size_t>(k)) {
        std::vector<double> dists(dataset.size(), 0);

        for (size_t i = 0; i < dataset.size(); ++i) {
            double minDist = std::numeric_limits<double>::max();
            for (const auto& centroid : centroids) {
                double dist = metric->calculate(dataset[i], centroid);
                minDist = std::min(minDist, dist);
            }
            dists[i] = minDist * minDist;
        }

        std::uniform_real_distribution<> disReal(0, std::accumulate(dists.begin(), dists.end(), 0.0));
        double rndNum = disReal(gen);

        // Find the index
        size_t idx = 0;
        for (; idx < dataset.size(); ++idx) {
            if (rndNum <= dists[idx])
                break;
            rndNum -= dists[idx];
        }

        centroids.push_back(dataset[idx]);
    }
}
