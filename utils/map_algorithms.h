#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "dag_algorithms.h"


namespace MapAlgorithms {

    void addSegmentToMap(TrapezoidalMap &map, Dag &dag, const cg3::Segment2d &segment);

    void sameTrapezoid(TrapezoidalMap &map, const cg3::Segment2d &segment, size_t trapezoidId, Dag &dag, size_t nodeId);
    void sameTrapezoidSamePointQ(TrapezoidalMap &map, const cg3::Segment2d &segment, size_t trapezoidAId, Dag & dag, size_t nodeId);
    void sameTrapezoidSamePointP(TrapezoidalMap &map, const cg3::Segment2d &segment, size_t trapezoidAId, Dag & dag, size_t nodeId);

    void differentTrapezoid(TrapezoidalMap &map, Dag &dag, std::vector<size_t> &intersected, const cg3::Segment2d &segment);
    void differentTrapezoidSamePointQ(TrapezoidalMap &map, Dag &dag, std::vector<size_t> &intersected, const cg3::Segment2d &segment);
    void differentTrapezoidSamePointP(TrapezoidalMap &map, Dag &dag, std::vector<size_t> &intersected, const cg3::Segment2d &segment);

    void deleteIntersectedTrapezoid(TrapezoidalMap & map, std::vector<size_t> &intersected);

    void addNewBottomTrapezoids(TrapezoidalMap & map, std::vector<size_t> &intersected, std::vector<size_t> &lower, size_t point1Id, size_t point2Id, size_t segmentId);
    void addNewTopTrapezoids(TrapezoidalMap & map, std::vector<size_t> &intersected, std::vector<size_t> &upper, size_t point1Id, size_t point2Id, size_t segmentId);
    void addNewBottomTrapezoidsSamePointQ(TrapezoidalMap & map, std::vector<size_t> &intersected, std::vector<size_t> &lower, size_t point1Id, size_t point2Id, size_t segmentId);
    void addNewTopTrapezoidsSamePointQ(TrapezoidalMap & map, std::vector<size_t> &intersected, std::vector<size_t> &upper, size_t point1Id, size_t point2Id, size_t segmentId);
    void addNewBottomTrapezoidsSamePointP(TrapezoidalMap & map, std::vector<size_t> &intersected, std::vector<size_t> &lower, size_t point1Id, size_t point2Id, size_t segmentId);
    void addNewTopTrapezoidsSamePointP(TrapezoidalMap & map, std::vector<size_t> &intersected, std::vector<size_t> &upper, size_t point1Id, size_t point2Id, size_t segmentId);

}

#endif // ALGORITHMS_H
