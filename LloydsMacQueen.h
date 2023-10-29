#include "Metric.h"

class LloydsMacQueen {
private:
    std::vector<Vector> centroids;
    Metric* metric;
    int k;

public:
    LloydsMacQueen(int kClusters, Metric* metricInstance, const std::vector<Vector>& initialCentroids);
    void fit(const std::vector<Vector>& dataset);
    Vector assign(const Vector& point) const;
    const std::vector<Vector>& getCentroids() const;
};
