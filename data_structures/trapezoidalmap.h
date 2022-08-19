#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H
#include "trapezoid.h"

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

    Node getPointByPosition(const int Position);
    Node getSegmentByPosition(const int Position);
    Node getTrapezoidByPosition(const int Position);
};

#endif // TRAPEZOIDALMAP_H
