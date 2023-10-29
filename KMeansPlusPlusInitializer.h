#include "Metric.h"

class KMeansPlusPlusInitializer {
private:
    Metric* metric;

public:
    KMeansPlusPlusInitializer(Metric* metricInstance) : metric(metricInstance) {}
    std::vector<Vector> initializeCentroids(const std::vector<Vector>& dataset, int kClusters);
};
