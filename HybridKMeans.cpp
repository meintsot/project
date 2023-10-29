#include <cfloat>
#include "HybridKMeans.h"

HybridKMeans::HybridKMeans(int numCentroids, Metric* metricInstance, int numOfHashTables, int numOfHashFunctions, int number_of_hypercube_dimensions, int max_number_M_hypercube, int number_of_probes, bool useLSHFlag, bool useHypercubeFlag, bool useLloydsMacQueenFlag, int dimension)
        : k(numCentroids), metric(metricInstance), useLSH(useLSHFlag), useHypercube(useHypercubeFlag), useLloydsMacQueen(useLloydsMacQueenFlag), tables(numOfHashTables), functions(numOfHashFunctions), dimension(dimension), number_of_hypercube_dimensions(number_of_hypercube_dimensions), max_number_M_hypercube(max_number_M_hypercube), number_of_probes(number_of_probes) {

    lsh = nullptr;
    hypercube = nullptr;
    lloydsMacQueen = nullptr;
}

HybridKMeans::~HybridKMeans() {
    if (lsh) delete lsh;
    if (hypercube) delete hypercube;
    if (lloydsMacQueen) delete lloydsMacQueen;
}

void HybridKMeans::fit(const std::vector<Vector>& dataset) {
    // Initialize centroids using KMeans++
    centroids = KMeansPlusPlusInitializer(metric).initializeCentroids(dataset, k);
    // Initialize the strategies here
    if (useLSH && !lsh) {
        lsh = new LSH(tables, functions, metric, number_of_probes, dimension);
        lsh->build(centroids);
    }
    if (useHypercube && !hypercube) {
        hypercube = new Hypercube(number_of_hypercube_dimensions, metric, max_number_M_hypercube, dimension);
        hypercube->build(centroids);
    }
    if (useLloydsMacQueen) {
        if (!lloydsMacQueen) {
            lloydsMacQueen = new LloydsMacQueen(k, metric, centroids);
        }
        lloydsMacQueen->fit(dataset);
        centroids = lloydsMacQueen->getCentroids();
        return; // Since clustering is already done by Lloyd's with MacQueen
    }

    bool converged = false;
    const double convergenceThreshold = 1e-6; // Adjust this value as needed

    while (!converged) {
        for (size_t i = 0; i < dataset.size(); ++i) {
            const auto& dataPoint = dataset[i];
            Vector closestCentroid;
            if (useLSH) {
                closestCentroid = lsh->nearestNeighbor(dataPoint);
            } else if (useHypercube) {
                closestCentroid = hypercube->nearestNeighbor(dataPoint);
            } else if (useLloydsMacQueen) {
                closestCentroid = lloydsMacQueen->assign(dataPoint);
            }

            // Finding the index of the closest centroid
            for (size_t j = 0; j < centroids.size(); ++j) {
                if (closestCentroid == centroids[j]) {
                    labels[i] = j;
                    break;
                }
            }
        }

        // Calculate new centroids based on the labels
        std::vector<Vector> newCentroids(k, Vector(dataset[0].size(), 0)); // Assuming all vectors have the same size
        std::vector<int> counts(k, 0);

        for (size_t i = 0; i < dataset.size(); ++i) {
            for (size_t j = 0; j < newCentroids[labels[i]].size(); ++j) {
                newCentroids[labels[i]][j] += dataset[i][j];
            }
            counts[labels[i]]++;
        }

        for (size_t i = 0; i < k; ++i) {
            if (counts[i] != 0) {
                for (size_t j = 0; j < newCentroids[i].size(); ++j) {
                    newCentroids[i][j] /= counts[i];
                }
            }
        }

        // Check for convergence: If centroids have not changed significantly, break out of the loop
        double maxDistanceMoved = 0.0;
        for (size_t i = 0; i < k; ++i) {
            double distance = metric->calculate(centroids[i], newCentroids[i]);
            if (distance > maxDistanceMoved) {
                maxDistanceMoved = distance;
            }
        }
        if (maxDistanceMoved < convergenceThreshold) {
            converged = true;
        }

        centroids = newCentroids; // Update centroids
    }
}

std::vector<double> HybridKMeans::calculateSilhouette(const std::vector<Vector>& dataset) {
    int n = dataset.size();
    std::vector<double> silhouetteValues(n);

    for(int i = 0; i < n; i++) {
        double a_i = 0.0;
        double b_i = DBL_MAX;
        int sameClusterCount = 0;

        for(int j = 0; j < n; j++) {
            if(i != j) {
                double distance = metric->calculate(dataset[i], dataset[j]);
                if(labels[i] == labels[j]) {
                    a_i += distance;
                    sameClusterCount++;
                } else {
                    if(distance < b_i) {
                        b_i = distance;
                    }
                }
            }
        }

        a_i = a_i / sameClusterCount;
        silhouetteValues[i] = (b_i - a_i) / std::max(a_i, b_i);
    }

    return silhouetteValues;
}
