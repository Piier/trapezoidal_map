#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H
#include "trapezoid.h"

class TrapezoidalMap{

protected:
    std::vector<cg3::Point2d> pointsTable;
    std::vector<cg3::Segment2d> segmentsTable;
    std::vector<Trapezoid> trapezoidsTable;

public:
    TrapezoidalMap();

    size_t addPoint(const cg3::Point2d & point);
    size_t addSegment(const cg3::Segment2d & segment);
    size_t addTrapezoid(const Trapezoid & trapezoid);

    cg3::Point2d& getPointByPosition(const size_t position);
    cg3::Segment2d& getSegmentByPosition(const size_t position);
    Trapezoid& getTrapezoidByPosition(const size_t position);

    size_t trapezoidVectorSize() const;
    size_t pointVectorSize() const;
    void clear();
};

#endif // TRAPEZOIDALMAP_H
