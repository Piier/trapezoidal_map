#include "dag_algorithms.h"

namespace DagAlgorithms {

/**
 * @brief updateDagSameTrapezoid Update the dag when the end-points of the segment are in the same trapezoid
 * @param dag The dag
 * @param nodeId The position of the node in the dag
 * @param idVector A vector of the ids of the elements in the trapezoidal map
 */
void updateDagSameTrapezoid(TrapezoidalMap & map, Dag &dag, size_t nodeId, std::vector<size_t> & idVector){

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

    //Adding the new nodes and updating the id in the trapezoid
    dag.addNode(nodeP2);
    dag.addNode(nodeS1);
    map.getTrapezoidByPosition(idVector[3]).setNodeId(dag.addNode(A));
    map.getTrapezoidByPosition(idVector[4]).setNodeId(dag.addNode(B));
    map.getTrapezoidByPosition(idVector[5]).setNodeId(dag.addNode(C));
    map.getTrapezoidByPosition(idVector[6]).setNodeId(dag.addNode(D));
}

/**
 * @brief updateDagSameTrapezoid Update the dag when the end-points of the segment are in the same trapezoid and the right end-point is the same as another point
 * @param dag The dag
 * @param nodeId The position of the node in the dag
 * @param idVector A vector of the ids of the elements in the trapezoidal map
 */
void updateDagSameTrapezoidSamePointQ(TrapezoidalMap & map, Dag &dag, size_t nodeId, std::vector<size_t> & idVector){

    size_t dagSize = dag.getSize();

    //New nodes
    Node & nodeP1 = dag.getNodeByPosition(nodeId);//NodeId
    Node nodeS1 = Node(idVector[0], Type::T_Segment, dagSize+2, dagSize+3);//dagSize
    Node A = Node(idVector[3], Type::T_Trapezoid);//dagSize+1
    Node B = Node(idVector[4], Type::T_Trapezoid);//dagSize+2
    Node C = Node(idVector[5], Type::T_Trapezoid);//dagSize+3

    nodeP1.setType(Type::T_Point);
    nodeP1.setElement(idVector[1]);
    nodeP1.setLeftChild(dagSize+1);
    nodeP1.setRightChild(dagSize);

    //Adding the new nodes and updating the id in the trapezoid
    dag.addNode(nodeS1);
    map.getTrapezoidByPosition(idVector[3]).setNodeId(dag.addNode(A));
    map.getTrapezoidByPosition(idVector[4]).setNodeId(dag.addNode(B));
    map.getTrapezoidByPosition(idVector[5]).setNodeId(dag.addNode(C));
}

/**
 * @brief updateDagSameTrapezoid Update the dag when the end-points of the segment are in the same trapezoid and the left end-point is the same as another point
 * @param dag The dag
 * @param nodeId The position of the node in the dag
 * @param idVector A vector of the ids of the elements in the trapezoidal map
 */
void updateDagSameTrapezoidSamePointP(TrapezoidalMap & map, Dag &dag, size_t nodeId, std::vector<size_t> & idVector){

    size_t dagSize = dag.getSize();

    //New nodes
    Node & nodeP2 = dag.getNodeByPosition(nodeId);//NodeId
    Node nodeS1 = Node(idVector[0], Type::T_Segment, dagSize+2, dagSize+3);//dagSize
    Node A = Node(idVector[3], Type::T_Trapezoid);//dagSize+1
    Node B = Node(idVector[4], Type::T_Trapezoid);//dagSize+2
    Node C = Node(idVector[5], Type::T_Trapezoid);//dagSize+3

    nodeP2.setType(Type::T_Point);
    nodeP2.setElement(idVector[2]);
    nodeP2.setLeftChild(dagSize);
    nodeP2.setRightChild(dagSize+1);

    //Adding the new nodes and updating the id in the trapezoid
    dag.addNode(nodeS1);
    map.getTrapezoidByPosition(idVector[3]).setNodeId(dag.addNode(A));
    map.getTrapezoidByPosition(idVector[4]).setNodeId(dag.addNode(B));
    map.getTrapezoidByPosition(idVector[5]).setNodeId(dag.addNode(C));
}

/**
 * @brief updateDagSameTrapezoid Update the dag when the end-points of the segment are in the same trapezoid and the end-points are already in the map
 * @param dag The dag
 * @param nodeId The position of the node in the dag
 * @param idVector A vector of the ids of the elements in the trapezoidal map
 */
void updateDagSameTrapezoidSamePointPQ(TrapezoidalMap & map, Dag &dag, size_t nodeId, std::vector<size_t> & idVector){

    size_t dagSize = dag.getSize();

    //New nodes
    Node & nodeS1 = dag.getNodeByPosition(nodeId);//NodeId
    Node A = Node(idVector[3], Type::T_Trapezoid);//dagSize+1
    Node B = Node(idVector[4], Type::T_Trapezoid);//dagSize+2

    nodeS1.setType(Type::T_Segment);
    nodeS1.setElement(idVector[0]);
    nodeS1.setLeftChild(dagSize);
    nodeS1.setRightChild(dagSize+1);

    //Adding the new nodes and updating the id in the trapezoid
    map.getTrapezoidByPosition(idVector[3]).setNodeId(dag.addNode(A));
    map.getTrapezoidByPosition(idVector[4]).setNodeId(dag.addNode(B));
}

/**
 * @brief updateDagDifferentTrapezoid Update the dag when the end-points of the segment are not in the same trapezoid
 * @param map Map
 * @param dag Dag
 * @param intersected The id of intersected trapezoid
 * @param upper The id of the new trapezoid above the segment
 * @param lower The id of the new trapezoid under the segment
 * @param ids The id of the new points and segment
 */
void updateDagDifferentTrapezoid(TrapezoidalMap &map, Dag &dag, std::vector<size_t> & intersected, std::vector<size_t> &upper, std::vector<size_t> &lower, std::vector<size_t> &ids){

    Node segment = Node(ids[2], Type::T_Segment);
    Node trapezoid = Node(intersected[0], Type::T_Trapezoid);
    std::vector<size_t> segmentId = std::vector<size_t>();

    //Update the trapezoid nodes into point nodes
    size_t idLeftPoint = map.getTrapezoidByPosition(intersected[0]).getNodeId();
    size_t idRightPoint = map.getTrapezoidByPosition(intersected.back()).getNodeId();;
    dag.getNodeByPosition(idLeftPoint).setElementAndType(ids[0],Type::T_Point);
    dag.getNodeByPosition(idRightPoint).setElementAndType(ids[1],Type::T_Point);

    //Inserting and updating the new segment nodes
    size_t i = 1;
    segmentId.push_back(dag.addNode(segment));//The first new segment node
    //Here i update the trapezoid nodes of the internal intersectd trapezoid into segment nodes
    while(i<intersected.size()-1){
        dag.getNodeByPosition(map.getTrapezoidByPosition(intersected[i]).getNodeId()).setElementAndType(ids[2], Type::T_Segment);
        i++;
    }
    segmentId.push_back(dag.addNode(segment));// The second new segment node


    //Inserting the new Trapezoid nodes
    map.getTrapezoidByPosition(intersected[0]).setNodeId(dag.addNode(trapezoid));//First trapezoid
    //Inserting the upper and lower trapezoid nodes
    i=0;
    while(i<upper.size()){
        trapezoid.setElement(upper[i]);
        map.getTrapezoidByPosition(upper[i]).setNodeId(dag.addNode(trapezoid));
        i++;
    }
    i=0;
    while(i<lower.size()){
        trapezoid.setElement(lower[i]);
        map.getTrapezoidByPosition(lower[i]).setNodeId(dag.addNode(trapezoid));
        i++;
    }
    trapezoid.setElement(intersected.back());
    map.getTrapezoidByPosition(intersected.back()).setNodeId(dag.addNode(trapezoid));//Last trapezoid

    //Now i set the children
    //First sub-tree (where left end-point lies)

    dag.getNodeByPosition(idLeftPoint).setChildren(map.getTrapezoidByPosition(intersected[0]).getNodeId(), segmentId[0]);
    dag.getNodeByPosition(segmentId[0]).setChildren(map.getTrapezoidByPosition(upper[0]).getNodeId(), map.getTrapezoidByPosition(lower[0]).getNodeId());
    //last sub-tree (where right end-pointlies)
    dag.getNodeByPosition(idRightPoint).setChildren(segmentId[1], map.getTrapezoidByPosition(intersected.back()).getNodeId());
    dag.getNodeByPosition(segmentId[1]).setChildren(map.getTrapezoidByPosition(upper.back()).getNodeId(), map.getTrapezoidByPosition(lower.back()).getNodeId());

    //Internal sub-trees
    i=1;//While
    size_t j=0;//Upper
    size_t k=0;//Lower

    double x;
    if(map.getPointByPosition(map.getTrapezoidByPosition(upper[0]).getRight()).x()<map.getPointByPosition(map.getTrapezoidByPosition(lower[0]).getRight()).x())
        j++;
    else
        k++;
    while(i<intersected.size()-1){


        dag.getNodeByPosition(map.getTrapezoidByPosition(intersected[i]).getNodeId()).setChildren(map.getTrapezoidByPosition(upper[j]).getNodeId(), map.getTrapezoidByPosition(lower[k]).getNodeId());

        x = map.getPointByPosition(map.getTrapezoidByPosition(intersected[i]).getRight()).x();
        if(map.getPointByPosition(map.getTrapezoidByPosition(upper[j]).getRight()).x()==x)
            j++;
        if(map.getPointByPosition(map.getTrapezoidByPosition(lower[k]).getRight()).x()==x)
            k++;

        i++;
    }
}

/**
 * @brief updateDagDifferentTrapezoid Update the dag when the end-points of the segment are not in the same trapezoid and the right end-point is already in
 * @param map Map
 * @param dag Dag
 * @param intersected The id of intersected trapezoid
 * @param upper The id of the new trapezoid above the segment
 * @param lower The id of the new trapezoid under the segment
 * @param ids The id of the new points and segment
 */
void updateDagDifferentTrapezoidSamePointQ(TrapezoidalMap &map, Dag &dag, std::vector<size_t> & intersected, std::vector<size_t> &upper, std::vector<size_t> &lower, std::vector<size_t> &ids){

    Node segment = Node(ids[2], Type::T_Segment);
    Node trapezoid = Node(intersected[0], Type::T_Trapezoid);
    std::vector<size_t> segmentId = std::vector<size_t>();

    //Update the trapezoid nodes into point nodes
    size_t idLeftPoint = map.getTrapezoidByPosition(intersected[0]).getNodeId();
    size_t idRightPoint = map.getTrapezoidByPosition(intersected.back()).getNodeId();//In this case is a segment
    dag.getNodeByPosition(idLeftPoint).setElementAndType(ids[0],Type::T_Point);
    dag.getNodeByPosition(idRightPoint).setElementAndType(ids[2],Type::T_Segment);

    //Inserting and updating the new segment nodes
    size_t i = 1;
    segmentId.push_back(dag.addNode(segment));//The first new segment node
    //Here i update the trapezoid nodes of the internal intersectd trapezoid into segment nodes
    while(i<intersected.size()-1){
        dag.getNodeByPosition(map.getTrapezoidByPosition(intersected[i]).getNodeId()).setElementAndType(ids[2], Type::T_Segment);
        i++;
    }



    //Inserting the new Trapezoid nodes
    map.getTrapezoidByPosition(intersected[0]).setNodeId(dag.addNode(trapezoid));//First trapezoid
    //Inserting the upper and lower trapezoid nodes
    i=0;
    while(i<upper.size()){
        trapezoid.setElement(upper[i]);
        map.getTrapezoidByPosition(upper[i]).setNodeId(dag.addNode(trapezoid));
        i++;
    }
    i=0;
    while(i<lower.size()){
        trapezoid.setElement(lower[i]);
        map.getTrapezoidByPosition(lower[i]).setNodeId(dag.addNode(trapezoid));
        i++;
    }

    //Now i set the children
    //First sub-tree (where left end-point lies)

    dag.getNodeByPosition(idLeftPoint).setChildren(map.getTrapezoidByPosition(intersected[0]).getNodeId(), segmentId[0]);
    dag.getNodeByPosition(segmentId[0]).setChildren(map.getTrapezoidByPosition(upper[0]).getNodeId(), map.getTrapezoidByPosition(lower[0]).getNodeId());
    //last sub-tree (where right end-pointlies)
    dag.getNodeByPosition(idRightPoint).setChildren(map.getTrapezoidByPosition(upper.back()).getNodeId(), map.getTrapezoidByPosition(lower.back()).getNodeId());

    //Internal sub-trees
    i=1;//While
    size_t j=0;//Upper
    size_t k=0;//Lower

    double x;
    if(map.getPointByPosition(map.getTrapezoidByPosition(upper[0]).getRight()).x()<map.getPointByPosition(map.getTrapezoidByPosition(lower[0]).getRight()).x())
        j++;
    else
        k++;
    while(i<intersected.size()-1){


        dag.getNodeByPosition(map.getTrapezoidByPosition(intersected[i]).getNodeId()).setChildren(map.getTrapezoidByPosition(upper[j]).getNodeId(), map.getTrapezoidByPosition(lower[k]).getNodeId());

        x = map.getPointByPosition(map.getTrapezoidByPosition(intersected[i]).getRight()).x();
        if(map.getPointByPosition(map.getTrapezoidByPosition(upper[j]).getRight()).x()==x)
            j++;
        if(map.getPointByPosition(map.getTrapezoidByPosition(lower[k]).getRight()).x()==x)
            k++;

        i++;
    }
}

/**
 * @brief updateDagDifferentTrapezoid Update the dag when the end-points of the segment are not in the same trapezoid and the left end-point is already in
 * @param map Map
 * @param dag Dag
 * @param intersected The id of intersected trapezoid
 * @param upper The id of the new trapezoid above the segment
 * @param lower The id of the new trapezoid under the segment
 * @param ids The id of the new points and segment
 */
void updateDagDifferentTrapezoidSamePointP(TrapezoidalMap &map, Dag &dag, std::vector<size_t> & intersected, std::vector<size_t> &upper, std::vector<size_t> &lower, std::vector<size_t> &ids){

    Node segment = Node(ids[2], Type::T_Segment);
    Node trapezoid = Node(intersected[0], Type::T_Trapezoid);
    std::vector<size_t> segmentId = std::vector<size_t>();

    //Update the trapezoid nodes into point nodes
    size_t idLeftPoint = map.getTrapezoidByPosition(intersected[0]).getNodeId();//In this case is a segment
    size_t idRightPoint = map.getTrapezoidByPosition(intersected.back()).getNodeId();;
    dag.getNodeByPosition(idLeftPoint).setElementAndType(ids[2],Type::T_Segment);
    dag.getNodeByPosition(idRightPoint).setElementAndType(ids[1],Type::T_Point);

    //Inserting and updating the new segment nodes
    size_t i = 1;

    //Here i update the trapezoid nodes of the internal intersectd trapezoid into segment nodes
    while(i<intersected.size()-1){
        dag.getNodeByPosition(map.getTrapezoidByPosition(intersected[i]).getNodeId()).setElementAndType(ids[2], Type::T_Segment);
        i++;
    }
    segmentId.push_back(dag.addNode(segment));// The last new segment node


    //Inserting the new Trapezoid nodes
    //Inserting the upper and lower trapezoid nodes
    i=0;
    while(i<upper.size()){
        trapezoid.setElement(upper[i]);
        map.getTrapezoidByPosition(upper[i]).setNodeId(dag.addNode(trapezoid));
        i++;
    }
    i=0;
    while(i<lower.size()){
        trapezoid.setElement(lower[i]);
        map.getTrapezoidByPosition(lower[i]).setNodeId(dag.addNode(trapezoid));
        i++;
    }
    trapezoid.setElement(intersected.back());
    map.getTrapezoidByPosition(intersected.back()).setNodeId(dag.addNode(trapezoid));//Last trapezoid

    //Now i set the children
    //First sub-tree (where left end-point lies)

    dag.getNodeByPosition(idLeftPoint).setChildren(map.getTrapezoidByPosition(upper[0]).getNodeId(), map.getTrapezoidByPosition(lower[0]).getNodeId());
    //last sub-tree (where right end-pointlies)
    dag.getNodeByPosition(idRightPoint).setChildren(segmentId[0], map.getTrapezoidByPosition(intersected.back()).getNodeId());
    dag.getNodeByPosition(segmentId[0]).setChildren(map.getTrapezoidByPosition(upper.back()).getNodeId(), map.getTrapezoidByPosition(lower.back()).getNodeId());

    //Internal sub-trees
    i=1;//While
    size_t j=0;//Upper
    size_t k=0;//Lower

    double x;
    if(map.getPointByPosition(map.getTrapezoidByPosition(upper[0]).getRight()).x()<map.getPointByPosition(map.getTrapezoidByPosition(lower[0]).getRight()).x())
        j++;
    else
        k++;
    while(i<intersected.size()-1){


        dag.getNodeByPosition(map.getTrapezoidByPosition(intersected[i]).getNodeId()).setChildren(map.getTrapezoidByPosition(upper[j]).getNodeId(), map.getTrapezoidByPosition(lower[k]).getNodeId());

        x = map.getPointByPosition(map.getTrapezoidByPosition(intersected[i]).getRight()).x();
        if(map.getPointByPosition(map.getTrapezoidByPosition(upper[j]).getRight()).x()==x)
            j++;
        if(map.getPointByPosition(map.getTrapezoidByPosition(lower[k]).getRight()).x()==x)
            k++;

        i++;
    }
}


/**
 * @brief queryDag Perform a search in the Dag
 * @param map The Trapezoidal map
 * @param dag The Dag
 * @param node The root node
 * @param point The point to search
 * @return The index of the trapezoid in the map
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
    if(AlgorithmsUtils::isPointOnTheLeft(s.p1(), s.p2(), point))
        return queryDag(map, dag, dag.getNodeByPosition(node.getLeftChild()), point);
    return queryDag(map, dag, dag.getNodeByPosition(node.getRightChild()), point);
}

}
