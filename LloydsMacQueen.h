#include "Metric.h"
#include <vector>

class LloydsMacQueen {
private:
    std::vector<Vector> centroids;
    Metric* metric;
    int k;

    void initializeCentroidsKMeansPlusPlus(const std::vector<Vector>& dataset);

public:
    LloydsMacQueen(int kClusters, Metric* metricInstance);
    void fit(const std::vector<Vector>& dataset);
    Vector assign(const Vector& point) const;
    const std::vector<Vector>& getCentroids() const;
};
