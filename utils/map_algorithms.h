#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "dag_algorithms.h"


namespace MapAlgorithms {

    void addSegmentToMap(TrapezoidalMap &map, Dag &dag, const cg3::Segment2d &segment);
    void sameTrapezoid(TrapezoidalMap &map, const cg3::Segment2d &segment, size_t trapezoidId, Dag &dag, size_t nodeId);
    void differentTrapezoid(TrapezoidalMap &map, std::vector<size_t> &intersected, const cg3::Segment2d &segment);

}

#endif // ALGORITHMS_H
