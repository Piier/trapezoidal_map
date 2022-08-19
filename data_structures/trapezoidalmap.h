#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H
#include "trapezoid.h"
#include <vector>
#include <cg3/geometry/point2.h>
#include <cg3/geometry/polygon2.h>

class TrapezoidalMap{

private:
    std::vector<cg3::Point2d> pointsTable;
    std::vector<cg3::Segment2d> segmentsTable;
    std::vector<Trapezoid> trapezoidTable;

public:
    TrapezoidalMap();

    void addPoint(const cg3::Point2d & point);
    void addSegment(const cg3::Segment2d & segment);
    void addTrapezoid(const Trapezoid & trapezoid);

    cg3::Point2d getPointByPosition(const int Position);
    cg3::Segment2d getSegmentByPosition(const int Position);
    Trapezoid getTrapezoidByPosition(const int Position);
};

#endif // TRAPEZOIDALMAP_H
