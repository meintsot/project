#include "LloydsMacQueen.h"
#include <limits>

LloydsMacQueen::LloydsMacQueen(int kClusters, Metric* metricInstance, const std::vector<Vector>& initialCentroids)
        : k(kClusters), metric(metricInstance), centroids(initialCentroids) {}

void LloydsMacQueen::fit(const std::vector<Vector>& dataset) {
    bool changed = true;

    while (changed) {
        changed = false;
        std::vector<int> assignments(dataset.size());

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

            // Update the centroid immediately
            Vector& currentCentroid = centroids[closestCluster];
            Vector dataPoint = dataset[i];
            for (size_t dim = 0; dim < dataPoint.size(); ++dim) {
                currentCentroid[dim] = ((assignments[i] * currentCentroid[dim]) + dataPoint[dim]) / (assignments[i] + 1);
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
