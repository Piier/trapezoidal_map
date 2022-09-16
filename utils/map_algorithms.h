#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "dag_algorithms.h"


namespace MapAlgorithms {

    void addSegmentToMap(TrapezoidalMap &map, Dag &dag, const cg3::Segment2d &segment);

    void sameTrapezoid(TrapezoidalMap &map, const cg3::Segment2d &segment, const size_t trapezoidId, Dag &dag, const size_t nodeId);
    void sameTrapezoidSamePointQ(TrapezoidalMap &map, const cg3::Segment2d &segment, const size_t trapezoidAId, Dag & dag, const size_t nodeId);
    void sameTrapezoidSamePointP(TrapezoidalMap &map, const cg3::Segment2d &segment, const size_t trapezoidAId, Dag & dag, const size_t nodeId);
    void sameTrapezoidSamePointPQ(TrapezoidalMap &map, const cg3::Segment2d &segment, const size_t trapezoidAId, Dag & dag, const size_t nodeId);

    void differentTrapezoid(TrapezoidalMap &map, Dag &dag, const std::vector<size_t> &intersected, const cg3::Segment2d &segment);
    void differentTrapezoidSamePointQ(TrapezoidalMap &map, Dag &dag, const std::vector<size_t> &intersected, const cg3::Segment2d &segment);
    void differentTrapezoidSamePointP(TrapezoidalMap &map, Dag &dag, const std::vector<size_t> &intersected, const cg3::Segment2d &segment);
    void differentTrapezoidSamePointPQ(TrapezoidalMap &map, Dag &dag, const std::vector<size_t> &intersected, const cg3::Segment2d &segment);

    void deleteIntersectedTrapezoid(TrapezoidalMap & map, const std::vector<size_t> &intersected);

    void addNewBottomTrapezoids(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &lower, const size_t point1Id, const size_t point2Id, const size_t segmentId);
    void addNewTopTrapezoids(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &upper, const size_t point1Id, const size_t point2Id, const size_t segmentId);
    void addNewBottomTrapezoidsSamePointQ(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &lower, const size_t point1Id, const size_t point2Id, const size_t segmentId);
    void addNewTopTrapezoidsSamePointQ(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &upper, const size_t point1Id, const size_t point2Id, const size_t segmentId);
    void addNewBottomTrapezoidsSamePointP(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &lower, const size_t point1Id, const size_t point2Id, const size_t segmentId);
    void addNewTopTrapezoidsSamePointP(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &upper, const size_t point1Id, const size_t point2Id, const size_t segmentId);
    void addNewBottomTrapezoidsSamePointPQ(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &lower, const size_t point1Id, const size_t point2Id, const size_t segmentId);
    void addNewTopTrapezoidsSamePointPQ(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &upper, const size_t point1Id, const size_t point2Id, const size_t segmentId);

    size_t findRealLeft(TrapezoidalMap & map, const size_t point, const cg3::Segment2d& segment);
    void followSegment(TrapezoidalMap &map, const cg3::Segment2d segment, std::vector<size_t>& intersected);

}

#endif // ALGORITHMS_H
