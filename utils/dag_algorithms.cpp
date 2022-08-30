#include "dag_algorithms.h"

namespace DagAlgorithms {

/**
 * @brief updateDagSameTrapezoid Update the dag when the end-points of the segment are in the same trapezoid
 * @param dag The dag
 * @param nodeId The position of the node in the dag
 * @param idVector A vector of the ids of the elements in the trapezoidal map
 */
void updateDagSameTrapezoid(Dag &dag, size_t nodeId, std::vector<size_t> & idVector){

    size_t dagSize = dag.getSize();

    //New nodes
    Node & nodeP1 = dag.getNodeByPosition(nodeId);//NodeId
    Node nodeP2 = Node(idVector[2], Type::T_Point, dagSize+1, dagSize+5);//dagSize
    Node nodeS1 = Node(idVector[0], Type::T_Segment, dagSize+3, dagSize+4);//dagSize+1
    Node A = Node(idVector[3], Type::T_Trapezoid);//dagSize+2
    Node B = Node(idVector[4], Type::T_Trapezoid);//dagSize+3
    Node C = Node(idVector[5], Type::T_Trapezoid);//dagSize+4
    Node D = Node(idVector[6], Type::T_Trapezoid);//dagSize+5

    nodeP1.setType(Type::T_Point);
    nodeP1.setElement(idVector[1]);
    nodeP1.setLeftChild(dagSize+2);
    nodeP1.setRightChild(dagSize);

    //Adding the new nodes
    dag.addNode(nodeP2);
    dag.addNode(nodeS1);
    dag.addNode(A);
    dag.addNode(B);
    dag.addNode(C);
    dag.addNode(D);
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
    if(AlgorithmsUtils::isPointOnTheLeft(s.p1(), s.p2(), point))
        return queryDag(map, dag, dag.getNodeByPosition(node.getLeftChild()), point);
    return queryDag(map, dag, dag.getNodeByPosition(node.getRightChild()), point);
}

}
