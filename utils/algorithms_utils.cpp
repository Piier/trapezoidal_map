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
 * @param newSegment A segment with the points in ascending order
 */
void checkSegment(const cg3::Segment2d& oldSegment, cg3::Segment2d &newSegment){

    if(oldSegment.p1().x()<=oldSegment.p2().x()){
        newSegment.setP1(oldSegment.p1());
        newSegment.setP2(oldSegment.p2());
    }else{
        newSegment.setP2(oldSegment.p1());
        newSegment.setP1(oldSegment.p2());
    }
}

/**
 * @brief getYGivenX Get the y value of a point that lies in segment given the x
 * @param segment The segment
 * @param x X value
 * @return The y value
 */
double getYGivenX(const cg3::Segment2d& segment, const double x){
    return (segment.p1().y()+(segment.p2().y()-segment.p1().y())/(segment.p2().x()-segment.p1().x())*(x-segment.p1().x()));
}


/**
 * @note https://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
 * @brief xorshf96 Random generator
 * @return A random number
 */
unsigned long xorshf96() {
    static unsigned long x=123456789, y=362436069, z=521288629;
    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

   return z;
}

/**
  @note https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison
 * @brief pointEssentiallyEqual
 * @param a Point a
 * @param b Point b
 * @return True if the points are the same
 */
bool pointEssentiallyEqual(const cg3::Point2d a, const cg3::Point2d b)
{
    if(std::fabs(a.x() - b.x()) <= std::numeric_limits<double>::epsilon()){
        if(std::fabs(a.y() - b.y()) <= std::numeric_limits<double>::epsilon())
            return true;
    }

    return false;
}
}
