#include "KMeansPlusPlusInitializer.h"
#include <random>
#include <numeric>
#include <algorithm>
#include <limits>

std::vector<Vector> KMeansPlusPlusInitializer::initializeCentroids(const std::vector<Vector>& dataset, int kClusters) {
    std::vector<Vector> centroids;
    std::random_device rd;
    std::mt19937 gen(rd());

    // 1. Choose one data point randomly
    std::uniform_int_distribution<> dis(0, dataset.size() - 1);
    centroids.push_back(dataset[dis(gen)]);

    while (centroids.size() < static_cast<size_t>(kClusters)) {
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

    return centroids;
}
