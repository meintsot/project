#pragma once
#include "Metric.h"

class EuclideanDistance : public Metric {
public:
    double calculate(const Vector& vec1, const Vector& vec2) const override;
};
