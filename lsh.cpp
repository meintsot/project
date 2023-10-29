#include "LSH.h"
#include <algorithm>
#include <random>
#include "EuclideanDistance.h"

LSH::LSH(int tables, int functions, Metric* metric1, int numberOfProbes, int dim)
        : numOfHashTables(tables), numOfHashFunctions(functions), metric(metric1), numberOfProbes(numberOfProbes), dimension(dim) {
    generateRandomVectors();
    hashTables.resize(numOfHashTables);
}

void LSH::generateRandomVectors() {
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    for (int i = 0; i < numOfHashTables * numOfHashFunctions; ++i) {
        Vector v;
        for (int j = 0; j < dimension; ++j) {  // Using the passed dimension
            v.push_back(distribution(generator));
        }
        randomVectors.push_back(v);
    }
}

std::vector<Vector> LSH::retrieveCandidates(const Vector& point) const {
    std::vector<Vector> candidates;
    for (int i = 0; i < numOfHashTables; ++i) {
        int hashValue = computeHashValue(point, i);
        for (int j = hashValue; j < hashValue + numberOfProbes && j < (1 << numOfHashFunctions); ++j) {
            if (hashTables[i].find(j) != hashTables[i].end()) {
                const auto& bucket = hashTables[i].at(j);
                candidates.insert(candidates.end(), bucket.begin(), bucket.end());
            }
        }
    }
    return candidates;
}

int LSH::computeHashValue(const Vector& point, int tableIndex) const {
    int hashValue = 0;
    for (int j = 0; j < numOfHashFunctions; ++j) {
        double dotProduct = metric->calculate(point, randomVectors[tableIndex * numOfHashFunctions + j]);
        if (dotProduct > 0) {
            hashValue |= (1 << j);
        }
    }
    return hashValue;
}

void LSH::build(const std::vector<Vector>& dataset) {
    hashTables.resize(numOfHashTables);

    for (const auto& dataPoint : dataset) {
        for (int i = 0; i < numOfHashTables; ++i) {
            int hashValue = computeHashValue(dataPoint, i);
            hashTables[i][hashValue].push_back(dataPoint);
        }
    }
}

Vector LSH::nearestNeighbor(const Vector& point) const {
    std::vector<Vector> candidates;

    // Find all possible candidate points from the hash tables
    for (int i = 0; i < numOfHashTables; ++i) {
        int hashValue = computeHashValue(point, i);

        // Now, not just the exact hash value bucket, but neighboring buckets as well, based on numberOfProbes
        for (int j = hashValue; j < hashValue + numberOfProbes && j < (1 << numOfHashFunctions); ++j) {
            if (hashTables[i].find(j) != hashTables[i].end()) {
                const auto& bucket = hashTables[i].at(j);
                candidates.insert(candidates.end(), bucket.begin(), bucket.end());
            }
        }
    }

    double minDist = std::numeric_limits<double>::max();
    Vector nearest;
    for (const auto& candidate : candidates) {
        double dist = metric->calculate(point, candidate);
        if (dist < minDist) {
            minDist = dist;
            nearest = candidate;
        }
    }

    return nearest;
}

std::vector<Vector> LSH::NClosestVectors(const Vector& point, int N) const {
    std::vector<Vector> candidates;

    // Find all possible candidate points from the hash tables
    for (int i = 0; i < numOfHashTables; ++i) {
        int hashValue = computeHashValue(point, i);

        for (int j = hashValue; j < hashValue + numberOfProbes && j < (1 << numOfHashFunctions); ++j) {
            if (hashTables[i].find(j) != hashTables[i].end()) {
                const auto& bucket = hashTables[i].at(j);
                candidates.insert(candidates.end(), bucket.begin(), bucket.end());
            }
        }
    }

    // Compute distances and sort
    std::vector<std::pair<double, Vector>> distanceAndCandidates;
    for (const auto& candidate : candidates) {
        double dist = metric->calculate(point, candidate);
        distanceAndCandidates.push_back({dist, candidate});
    }

    std::sort(distanceAndCandidates.begin(), distanceAndCandidates.end(),
              [](const std::pair<double, Vector>& a, const std::pair<double, Vector>& b) {
                  return a.first < b.first;
              });

    std::vector<Vector> result;
    int count = 0;
    for (const auto& [dist, vec] : distanceAndCandidates) {
        if (count >= N) break;
        result.push_back(vec);
        count++;
    }

    return result;
}

std::vector<Vector> LSH::rangeSearch(const Vector& point, double range) const {
    std::vector<Vector> candidates, result;

    // Find all possible candidate points from the hash tables
    for (int i = 0; i < numOfHashTables; ++i) {
        int hashValue = computeHashValue(point, i);

        for (int j = hashValue; j < hashValue + numberOfProbes && j < (1 << numOfHashFunctions); ++j) {
            if (hashTables[i].find(j) != hashTables[i].end()) {
                const auto& bucket = hashTables[i].at(j);
                candidates.insert(candidates.end(), bucket.begin(), bucket.end());
            }
        }
    }

    for (const auto& candidate : candidates) {
        if (metric->calculate(point, candidate) <= range) {
            result.push_back(candidate);
        }
    }

    return result;
}
