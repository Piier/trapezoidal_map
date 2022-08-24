#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "data_structures/trapezoidalmap.h"
#include "data_structures/dag.h"

namespace Algorithms {

    void addSegmentToMap(TrapezoidalMap &map, Dag &dag, const cg3::Segment2d &segment);
    size_t queryDag(TrapezoidalMap &map, Dag &dag, Node &node, const cg3::Point2d & point);
    bool isPointOnTheLeft(const cg3::Point2d & segm1, const cg3::Point2d & segm2, const cg3::Point2d & point);
}

#endif // ALGORITHMS_H
