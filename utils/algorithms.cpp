#include "algorithms.h"

namespace Algorithms {

void addSegmentToMap(TrapezoidalMap &map, Dag &dag, const cg3::Segment2d &segment){
    map.addSegment(segment);
    map.addPoint(segment.p1());
    map.addPoint(segment.p2());
}


/**
 * @brief queryDag Perform a search in the Dag
 * @param map The Trapezoidal map
 * @param dag The Dag
 * @param node The root node
 * @param point The point to search
 * @return The index of the Trapezoid in the map
 */
size_t queryDag(TrapezoidalMap &map, Dag & dag, Node &node, const cg3::Point2d &point){

    Type type = node.getType();

    //The node is a leaf
    if(type==Type::T_Trapezoid)
        return node.getElement();

    //The node is a point
    if(type==Type::T_Point){
        if(point.x()<=map.getPointByPosition(node.getElement()).x())
            return queryDag(map, dag, dag.getNodeByPosition(node.getLeftChild()), point);
        return queryDag(map, dag, dag.getNodeByPosition(node.getRightChild()), point);
    }

    //The node is a segment
    cg3::Segment2d & s = map.getSegmentByPosition(node.getElement());
    if(isPointOnTheLeft(s.p1(), s.p2(), point))
        return queryDag(map, dag, dag.getNodeByPosition(node.getLeftChild()), point);
    return queryDag(map, dag, dag.getNodeByPosition(node.getRightChild()), point);
}


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

}
