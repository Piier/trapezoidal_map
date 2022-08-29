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

/**
 * @brief followSegment Return a list of trapezoid intersected by the segment s.
 * @param map The map
 * @param segment The segment
 * @param intersected The vector (it must contain the starting trapezoid) that will contain the trapezoids
 */
void followSegment(TrapezoidalMap& map, const cg3::Segment2d segment, std::vector<size_t>& intersected){
    size_t i = 0;

    while(segment.p2().x()>map.getPointByPosition(map.getTrapezoidByPosition(intersected[i]).getRight()).x()){
        if(isPointOnTheLeft(segment.p1(), segment.p2(), map.getPointByPosition(map.getTrapezoidByPosition(intersected[i]).getRight()))){
            intersected.push_back(map.getTrapezoidByPosition(intersected[i]).getAdjBottomRight());
        }else{
            intersected.push_back(map.getTrapezoidByPosition(intersected[i]).getAdjTopRight());
        }
        i++;
    }

}
}
