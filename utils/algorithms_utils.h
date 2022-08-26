#ifndef ALGORITHMS_UTILS_H
#define ALGORITHMS_UTILS_H
#include "data_structures/trapezoidalmap.h"
#include "data_structures/dag.h"

namespace AlgorithmsUtils {

    bool isPointOnTheLeft(const cg3::Point2d & segm1, const cg3::Point2d & segm2, const cg3::Point2d & point);
    cg3::Segment2d& checkSegment(const cg3::Segment2d& s);
}

#endif // ALGORITHMS_UTILS_H
