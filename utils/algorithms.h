#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "dag_algorithms.h"


namespace Algorithms {

    void addSegmentToMap(TrapezoidalMap &map, Dag &dag, const cg3::Segment2d &segment);
    void sameTrapezoid(TrapezoidalMap &map, const cg3::Segment2d &segment, size_t trapezoidId, Dag &dag, size_t nodeId);


    size_t queryDag(TrapezoidalMap &map, Dag &dag, Node &node, const cg3::Point2d & point);
    bool isPointOnTheLeft(const cg3::Point2d & segm1, const cg3::Point2d & segm2, const cg3::Point2d & point);


    cg3::Segment2d& checkSegment(const cg3::Segment2d& s);
}

#endif // ALGORITHMS_H
