#include "map_algorithms.h"

namespace MapAlgorithms {

void addSegmentToMap(TrapezoidalMap &map, Dag &dag, const cg3::Segment2d &segment){
    cg3::Segment2d tempSegment = cg3::Segment2d();

    AlgorithmsUtils::checkSegment(segment, tempSegment);

    size_t t1 = DagAlgorithms::queryDag(map, dag, dag.getRoot(), segment.p1());
    size_t t2 = DagAlgorithms::queryDag(map, dag, dag.getRoot(), segment.p2());

    if(t1==t2)
        sameTrapezoid(map, tempSegment, dag.getNodeByPosition(t1).getElement(), dag, t1);
    else{
        std::vector<size_t> intersected = std::vector<size_t>();
        intersected.push_back(dag.getNodeByPosition(t1).getElement());
        AlgorithmsUtils::followSegment(map, segment, intersected);
        differentTrapezoid(map, intersected, tempSegment);
        int i;
        i=0;
    }

}


void differentTrapezoid(TrapezoidalMap &map, std::vector<size_t> &intersected, const cg3::Segment2d &segment){
    std::vector<size_t> upper = std::vector<size_t>();
    std::vector<size_t> lower = std::vector<size_t>();

    //Adding the segment and points in the map
    size_t segmentId = map.addSegment(segment);
    size_t point1Id = map.addPoint(segment.p1());
    size_t point2Id = map.addPoint(segment.p2());

    int i = 0;
    int start = 0;
    Trapezoid temp;

    //Creating all the top trapezoid (the trapezoid on the top of the segment)
    while(i<(intersected.size()-1)){
        //The trapezoid is created only when the adj trapezoid has different top
        if(map.getTrapezoidByPosition(intersected[i]).getTop()!=map.getTrapezoidByPosition(intersected[i+1]).getTop()){

            temp = Trapezoid();
            temp.setTop(map.getTrapezoidByPosition(intersected[i]).getTop());
            temp.setBottom(segmentId);
            temp.setRight(map.getTrapezoidByPosition(intersected[i]).getRight());
            if(start==0)//If it is the first trapezoid is setted the left end-point as leftP
                temp.setLeft(point1Id);
            else
                temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());


            upper.push_back(map.addTrapezoid(temp));
            start=i+1;
        }
        i++;
    }
    //Same as in the cycle but now we are considering also the last intersected trapezoid,so the rightP is the right end-point
    temp = Trapezoid();
    temp.setTop(map.getTrapezoidByPosition(intersected[i]).getTop());
    temp.setBottom(segmentId);
    temp.setRight(point2Id);
    if(start==0)
        temp.setLeft(point1Id);
    else
        temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
    upper.push_back(map.addTrapezoid(temp));

    i = 0;
    start = 0;

    //Same but with the bottom trapezoid
    while(i<(intersected.size()-1)){
        if(map.getTrapezoidByPosition(intersected[i]).getBottom()!=map.getTrapezoidByPosition(intersected[i+1]).getBottom()){

            temp = Trapezoid();
            temp.setBottom(map.getTrapezoidByPosition(intersected[i]).getBottom());
            temp.setTop(segmentId);
            temp.setRight(map.getTrapezoidByPosition(intersected[i]).getRight());
            if(start==0)
                temp.setLeft(point1Id);
            else
                temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());

            lower.push_back(map.addTrapezoid(temp));
            start=i+1;
        }
        i++;
    }
    temp = Trapezoid();
    temp.setBottom(map.getTrapezoidByPosition(intersected[i]).getBottom());
    temp.setTop(segmentId);
    temp.setRight(point2Id);
    if(start==0)
        temp.setLeft(point1Id);
    else
        temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
    lower.push_back(map.addTrapezoid(temp));

    //Update the first and the last trapezoid
    Trapezoid &tA = map.getTrapezoidByPosition(intersected[0]);
    Trapezoid &tB = map.getTrapezoidByPosition(intersected.back());

    tA.setRight(point1Id);
    tA.setAdjTopRight(upper[0]);
    tA.setAdjBottomRight(lower[0]);

    tB.setLeft(point2Id);
    tB.setAdjTopLeft(upper.back());
    tB.setAdjBottomLeft(lower.back());

    //Delete the intersected trapezoid from the map (not the first and the last, they're new trapezoid now)
    i = 1;
    while(i<intersected.size()-1){
        map.getTrapezoidByPosition(intersected[i]).setDeleted();
        i++;
    }
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


}
