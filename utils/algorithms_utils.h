#ifndef ALGORITHMS_UTILS_H
#define ALGORITHMS_UTILS_H
#include "data_structures/trapezoidalmap.h"
#include "data_structures/dag.h"

namespace AlgorithmsUtils {

    bool isPointOnTheLeft(const cg3::Point2d & segm1, const cg3::Point2d & segm2, const cg3::Point2d & point);
    void checkSegment(const cg3::Segment2d& oldSegment, cg3::Segment2d& newSegment);
    void followSegment(TrapezoidalMap& map, const cg3::Segment2d segment, std::vector<size_t>& intersected);
    double getYGivenX(const cg3::Segment2d& segment, const double x);
}

#endif // ALGORITHMS_UTILS_H
