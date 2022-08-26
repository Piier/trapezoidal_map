#include "algorithms_utils.h"

namespace AlgorithmsUtils {

/**
 * @brief isPointOnTheLeft Return true if the point is on the left of a segment
 * @param p1 The leftmost point of a segment
 * @param p2 The rightmost point of a segment
 * @param p The point
 * @return True if the point is on the left, false in it is on the right
 */
bool isPointOnTheLeft(const cg3::Point2d &p1, const cg3::Point2d &p2, const cg3::Point2d &p){
   return ((p2.x()-p1.x())*(p.y()-p1.y()))-((p.x()-p1.x())*(p2.y()-p1.y()))>0;
}

/**
 * @brief checkSegment Check if the segment points are in ascending order
 * @param s The segment
 * @return A segment with the points in ascending order
 */
cg3::Segment2d& checkSegment(const cg3::Segment2d& s){
    cg3::Segment2d *segment = new cg3::Segment2d();

    if(s.p1().x()<=s.p2().x()){
        segment->setP1(s.p1());
        segment->setP2(s.p2());
    }else{
        segment->setP2(s.p1());
        segment->setP1(s.p2());
    }
    return *segment;
}
}
