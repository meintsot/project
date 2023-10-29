#include "Hypercube.h"
#include <algorithm>

Hypercube::Hypercube(int functions, Metric* metric1)
        : numOfHashFunctions(functions), metric(metric1) {
    generateRandomVectors();
}

void Hypercube::generateRandomVectors() {
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    for (int i = 0; i < numOfHashFunctions; ++i) {
        Vector v;
        for (int j = 0; j < 28 * 28; ++j) {  // Assuming MNIST image size
            v.push_back(distribution(generator));
        }
        randomVectors.push_back(v);
    }
}

std::string Hypercube::computeBinaryCode(const Vector& point) const {
    std::string code = "";
    for (int i = 0; i < numOfHashFunctions; ++i) {
        double dotProduct = metric->calculate(point, randomVectors[i]);
        code += (dotProduct > 0) ? '1' : '0';
    }
    return code;
}

void Hypercube::build(const std::vector<Vector>& dataset) {
    for (const auto& dataPoint : dataset) {
        std::string code = computeBinaryCode(dataPoint);
        hypercubeCells[code].push_back(dataPoint);
    }
}

Vector Hypercube::nearestNeighbor(const Vector& point) const {
    std::string code = computeBinaryCode(point);

    // Start with candidates from the current cell
    std::vector<Vector> allCandidates;
    if (hypercubeCells.find(code) != hypercubeCells.end()) {
        allCandidates = hypercubeCells.at(code);
    }

    // Get neighboring cells' candidates
    for (size_t i = 0; i < code.size(); ++i) {
        std::string neighborCode = code;
        neighborCode[i] = (neighborCode[i] == '0') ? '1' : '0'; // Flip the bit to get neighbor

        if (hypercubeCells.find(neighborCode) != hypercubeCells.end()) {
            const auto& neighborCandidates = hypercubeCells.at(neighborCode);
            allCandidates.insert(allCandidates.end(), neighborCandidates.begin(), neighborCandidates.end());
        }
    }

    // Find the nearest among all candidates
    double minDist = std::numeric_limits<double>::max();
    Vector nearest;
    for (const auto& candidate : allCandidates) {
        double dist = metric->calculate(point, candidate);
        if (dist < minDist) {
            minDist = dist;
            nearest = candidate;
        }
    }

    return nearest;
}

std::vector<Vector> Hypercube::NClosestVectors(const Vector& point, int N) const {
    std::string code = computeBinaryCode(point);

    std::vector<Vector> allCandidates = hypercubeCells.at(code);

    // Get neighboring cells' candidates
    for (size_t i = 0; i < code.size(); ++i) {
        std::string neighborCode = code;
        neighborCode[i] = (neighborCode[i] == '0') ? '1' : '0'; // Flip the bit to get neighbor

        if (hypercubeCells.find(neighborCode) != hypercubeCells.end()) {
            const auto& neighborCandidates = hypercubeCells.at(neighborCode);
            allCandidates.insert(allCandidates.end(), neighborCandidates.begin(), neighborCandidates.end());
        }
    }

    // Compute distances and sort
    std::vector<std::pair<double, Vector>> distanceAndCandidates;
    for (const auto& candidate : allCandidates) {
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

std::vector<Vector> Hypercube::rangeSearch(const Vector& point, double range) const {
    std::string code = computeBinaryCode(point);

    std::vector<Vector> allCandidates = hypercubeCells.at(code);

    // Get neighboring cells' candidates
    for (size_t i = 0; i < code.size(); ++i) {
        std::string neighborCode = code;
        neighborCode[i] = (neighborCode[i] == '0') ? '1' : '0'; // Flip the bit to get neighbor

        if (hypercubeCells.find(neighborCode) != hypercubeCells.end()) {
            const auto& neighborCandidates = hypercubeCells.at(neighborCode);
            allCandidates.insert(allCandidates.end(), neighborCandidates.begin(), neighborCandidates.end());
        }
    }

    std::vector<Vector> result;
    for (const auto& candidate : allCandidates) {
        if (metric->calculate(point, candidate) <= range) {
            result.push_back(candidate);
        }
    }

    return result;
}
