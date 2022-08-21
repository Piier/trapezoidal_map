#include "trapezoidalmap.h"

/**
 * @brief TrapezoidalMap::TrapezoidalMap Constructor
 */
TrapezoidalMap::TrapezoidalMap(){
    pointsTable=std::vector<cg3::Point2d>();
    segmentsTable=std::vector<cg3::Segment2d>();
    trapezoidsTable=std::vector<Trapezoid>();
}

/**
 * @brief TrapezoidalMap::addPoint Add a point to the trapezoidal map
 * @param point The point to add
 */
void TrapezoidalMap::addPoint(const cg3::Point2d &point){
    pointsTable.push_back(point);
}

/**
 * @brief TrapezoidalMap::addSegment Add a segment to the trapezoidal map
 * @param segment The segment to add
 */
void TrapezoidalMap::addSegment(const cg3::Segment2d &segment){
    segmentsTable.push_back(segment);
}

/**
 * @brief TrapezoidalMap::addTrapezoid Add a trapezoid to the trapezoidal map
 * @param trapezoid The trapezoid to add
 */
void TrapezoidalMap::addTrapezoid(const Trapezoid &trapezoid){
    trapezoidsTable.push_back(trapezoid);
}


/**
 * @brief TrapezoidalMap::getPointByPosition Get a point reference by is position
 * @param position The point position
 * @return The reference to the point at the given position
 */
cg3::Point2d& TrapezoidalMap::getPointByPosition(const size_t position){
    return pointsTable[position];
}

/**
 * @brief TrapezoidalMap::getSegmentByPosition Get a segment reference by is position
 * @param position The segment position
 * @return The reference to the segment at the given position
 */
cg3::Segment2d& TrapezoidalMap::getSegmentByPosition(const size_t position){
    return segmentsTable[position];
}

/**
 * @brief TrapezoidalMap::getTrapezoidByPosition Get a trapezoid pointer by is position if it isn't deleted
 * @param position The trapezoid position
 * @return The pointer to the trapezoid at the given position or nullptr
 */
Trapezoid* TrapezoidalMap::getTrapezoidByPosition(const size_t position){
    if(trapezoidsTable[position].isDeleted())
        return nullptr;

    return &trapezoidsTable[position];
}
