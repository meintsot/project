#include "EuclideanDistance.h"
#include <cmath>
#include <stdexcept>

double EuclideanDistance::calculate(const Vector& vec1, const Vector& vec2) const {
    if (vec1.size() != vec2.size()) {
        throw std::runtime_error("Vectors have different dimensions!");
    }
    double sum = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        double diff = static_cast<double>(vec1[i]) - static_cast<double>(vec2[i]);
        sum += diff * diff;
    }
    return std::sqrt(sum);
}
