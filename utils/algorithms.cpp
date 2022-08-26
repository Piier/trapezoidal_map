#include "algorithms.h"

namespace Algorithms {

void addSegmentToMap(TrapezoidalMap &map, Dag &dag, const cg3::Segment2d &segment){
    cg3::Segment2d& tempSegment = checkSegment(segment);

    size_t t1 = queryDag(map, dag, dag.getRoot(), segment.p1());
    size_t t2 = queryDag(map, dag, dag.getRoot(), segment.p2());

    if(t1==t2)
        sameTrapezoid(map, tempSegment, dag.getNodeByPosition(t1).getElement(), dag, t1);

    delete &tempSegment;
}


/**
 * @brief sameTrapezoid Update the trapezoidal map and the dag when the end-points of the segment are in the same trapezoid
 * @param map The map
 * @param segment The segment
 * @param trapezoidAId The position of the trapezoid in the map
 * @param dag The dag
 * @param nodeId The position of the node in the dag
 */
void sameTrapezoid(TrapezoidalMap &map, const cg3::Segment2d &segment, size_t trapezoidAId, Dag & dag, size_t nodeId){


    std::vector<size_t> idVector = std::vector<size_t>();
    Trapezoid & t = map.getTrapezoidByPosition(trapezoidAId);

    //Adding the segment, points and trapezoids in the map
    size_t segmentId = map.addSegment(segment);
    size_t point1Id = map.addPoint(segment.p1());
    size_t point2Id = map.addPoint(segment.p2());
    size_t trapezoidBId = map.addTrapezoid(Trapezoid(point1Id, point2Id, t.getTop(), segmentId));
    size_t trapezoidCId = map.addTrapezoid(Trapezoid(point1Id, point2Id, segmentId, t.getBottom()));
    size_t trapezoidDId= map.addTrapezoid(Trapezoid(point2Id, t.getRight(), t.getTop(), t.getBottom()));

    idVector.push_back(segmentId);
    idVector.push_back(point1Id);
    idVector.push_back(point2Id);
    idVector.push_back(trapezoidAId);
    idVector.push_back(trapezoidBId);
    idVector.push_back(trapezoidCId);
    idVector.push_back(trapezoidDId);

    //Manipulating the trapezoids
    Trapezoid & trapezoidA = map.getTrapezoidByPosition(trapezoidAId);
    Trapezoid & trapezoidB = map.getTrapezoidByPosition(trapezoidBId);
    Trapezoid & trapezoidC = map.getTrapezoidByPosition(trapezoidCId);
    Trapezoid & trapezoidD = map.getTrapezoidByPosition(trapezoidDId);

    trapezoidB.setAdjTopLeft(trapezoidAId);
    trapezoidB.setAdjBottomLeft(SIZE_MAX);
    trapezoidB.setAdjTopRight(trapezoidDId);
    trapezoidB.setAdjBottomRight(SIZE_MAX);

    trapezoidC.setAdjTopLeft(SIZE_MAX);
    trapezoidC.setAdjBottomLeft(trapezoidAId);
    trapezoidC.setAdjTopRight(SIZE_MAX);
    trapezoidC.setAdjBottomRight(trapezoidDId);

    trapezoidD.setAdjTopLeft(trapezoidBId);
    trapezoidD.setAdjBottomLeft(trapezoidCId);
    trapezoidD.setAdjTopRight(trapezoidA.getAdjTopRight());
    trapezoidD.setAdjBottomRight(trapezoidA.getAdjBottomRight());

    trapezoidA.setRight(point1Id);
    trapezoidA.setAdjTopRight(trapezoidBId);
    trapezoidA.setAdjBottomRight(trapezoidCId);

    //Update dag
    DagAlgorithms::updateDagSameTrapezoid(dag, nodeId, idVector);
}


/**
 * @brief queryDag Perform a search in the Dag
 * @param map The Trapezoidal map
 * @param dag The Dag
 * @param node The root node
 * @param point The point to search
 * @return The index of the node in the dag
 */
size_t queryDag(TrapezoidalMap &map, Dag & dag, Node &node, const cg3::Point2d &point){

    Type type = node.getType();

    //The node is a leaf
    if(type==Type::T_Trapezoid)
        return node.getId();

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
