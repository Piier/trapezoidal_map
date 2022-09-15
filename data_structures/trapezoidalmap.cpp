#include "trapezoidalmap.h"
#define BOUNDINGBOX 1e+6

/**
 * @brief TrapezoidalMap::TrapezoidalMap Constructor
 */
TrapezoidalMap::TrapezoidalMap(){
    pointsTable=std::vector<cg3::Point2d>();
    segmentsTable=std::vector<cg3::Segment2d>();
    trapezoidsTable=std::vector<Trapezoid>();

    pointsTable.push_back(cg3::Point2d(-BOUNDINGBOX,BOUNDINGBOX));
    pointsTable.push_back(cg3::Point2d(BOUNDINGBOX,BOUNDINGBOX));
    pointsTable.push_back(cg3::Point2d(-BOUNDINGBOX,-BOUNDINGBOX));
    pointsTable.push_back(cg3::Point2d(BOUNDINGBOX,-BOUNDINGBOX));
    segmentsTable.push_back(cg3::Segment2d(cg3::Point2d(-BOUNDINGBOX,BOUNDINGBOX),cg3::Point2d(BOUNDINGBOX,BOUNDINGBOX)));
    segmentsTable.push_back(cg3::Segment2d(cg3::Point2d(-BOUNDINGBOX,-BOUNDINGBOX),cg3::Point2d(BOUNDINGBOX,-BOUNDINGBOX)));
    trapezoidsTable.push_back(Trapezoid());
}

/**
 * @brief TrapezoidalMap::addPoint Add a point to the trapezoidal map
 * @param point The point to add
 * @return The position in the map
 */
size_t TrapezoidalMap::addPoint(const cg3::Point2d &point){
    pointsTable.push_back(point);
    return pointsTable.size()-1;
}

/**
 * @brief TrapezoidalMap::addSegment Add a segment to the trapezoidal map
 * @param segment The segment to add
 * @return The position in the map
 */
size_t TrapezoidalMap::addSegment(const cg3::Segment2d &segment){
    segmentsTable.push_back(segment);
    return segmentsTable.size()-1;
}

/**
 * @brief TrapezoidalMap::addTrapezoid Add a trapezoid to the trapezoidal map
 * @param trapezoid The trapezoid to add
 * @return The position in the map
 */
size_t TrapezoidalMap::addTrapezoid(const Trapezoid &trapezoid){
    trapezoidsTable.push_back(trapezoid);
    return trapezoidsTable.size()-1;
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
 * @brief TrapezoidalMap::getTrapezoidByPosition Get a trapezoid pointer by is position
 * @param position The trapezoid position
 * @return The pointer to the trapezoid at the given position
 */
Trapezoid& TrapezoidalMap::getTrapezoidByPosition(const size_t position){
    return trapezoidsTable[position];
}


/**
 * @brief TrapezoidalMap::trapezoidVectorSize Return he size of the vector that contains the trapezoids
 * @return The size of the vector that contains the trapezoids
 */
size_t TrapezoidalMap::trapezoidVectorSize() const{
    return trapezoidsTable.size();
}

/**
 * @brief TrapezoidalMap::trapezoidVectorSize Return he size of the vector that contains the points
 * @return The size of the vector that contains the trapezoids
 */
size_t TrapezoidalMap::pointVectorSize() const{
    return pointsTable.size();
}



void TrapezoidalMap::clear(){
    pointsTable.clear();
    segmentsTable.clear();
    trapezoidsTable.clear();

    //Re-initialize
    pointsTable.push_back(cg3::Point2d(-BOUNDINGBOX,BOUNDINGBOX));
    pointsTable.push_back(cg3::Point2d(BOUNDINGBOX,BOUNDINGBOX));
    pointsTable.push_back(cg3::Point2d(-BOUNDINGBOX,-BOUNDINGBOX));
    pointsTable.push_back(cg3::Point2d(BOUNDINGBOX,-BOUNDINGBOX));
    segmentsTable.push_back(cg3::Segment2d(cg3::Point2d(-BOUNDINGBOX,BOUNDINGBOX),cg3::Point2d(BOUNDINGBOX,BOUNDINGBOX)));
    segmentsTable.push_back(cg3::Segment2d(cg3::Point2d(-BOUNDINGBOX,-BOUNDINGBOX),cg3::Point2d(BOUNDINGBOX,-BOUNDINGBOX)));
    trapezoidsTable.push_back(Trapezoid());
}
