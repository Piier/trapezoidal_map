#include "map_algorithms.h"

namespace MapAlgorithms {

/**
 * @brief addSegmentToMap Add a segment to tha map and update the dag
 * @param map Map
 * @param dag Dag
 * @param segment Segment
 */
void addSegmentToMap(TrapezoidalMap &map, Dag &dag, const cg3::Segment2d &segment){
    cg3::Segment2d tempSegment = cg3::Segment2d();

    AlgorithmsUtils::checkSegment(segment, tempSegment);

    size_t t1 = DagAlgorithms::queryDag(map, dag, dag.getRoot(), tempSegment.p1());
    size_t t2 = DagAlgorithms::queryDag(map, dag, dag.getRoot(), tempSegment.p2());
    bool samePointP1=AlgorithmsUtils::pointEssentiallyEqual((map.getPointByPosition(map.getTrapezoidByPosition(t1).getRight())),tempSegment.p1());
    bool samePointP2=AlgorithmsUtils::pointEssentiallyEqual((map.getPointByPosition(map.getTrapezoidByPosition(t2).getRight())),tempSegment.p2());

    if(samePointP1)
        t1=findRealLeft(map, map.getTrapezoidByPosition(t1).getRight(), tempSegment);

    //end-points already in the map
    if(samePointP1 && samePointP2){
        //Same trapezoid
        if(t2==t1 || map.getTrapezoidByPosition(t1).getRight() == map.getTrapezoidByPosition(t2).getRight())
            sameTrapezoidSamePointPQ(map, tempSegment, t1, dag, map.getTrapezoidByPosition(t1).getNodeId());
        else{
            std::vector<size_t> intersected = std::vector<size_t>();
            intersected.push_back(t1);
            followSegment(map, tempSegment, intersected);
            differentTrapezoidSamePointPQ(map, dag, intersected, tempSegment);
        }
    }


    //Right end-point is the same as another point
    if(samePointP2 && !samePointP1){
        //Same trapezoid
        if(t1==t2 || map.getTrapezoidByPosition(t1).getRight() == map.getTrapezoidByPosition(t2).getRight() ){
            sameTrapezoidSamePointQ(map, tempSegment, t1, dag, map.getTrapezoidByPosition(t1).getNodeId());
        }else{
            std::vector<size_t> intersected = std::vector<size_t>();
            intersected.push_back(t1);
            followSegment(map, tempSegment, intersected);
            differentTrapezoidSamePointQ(map, dag, intersected, tempSegment);
        }
    }

    //Left end-point is the same as another point
    if(samePointP1 && !samePointP2){
        //Same trapezoid
        if(map.getTrapezoidByPosition(t1).getRight()==map.getTrapezoidByPosition(t2).getRight()){
            sameTrapezoidSamePointP(map, tempSegment, t2, dag, map.getTrapezoidByPosition(t2).getNodeId());
        }else{//Different trapezoid
            std::vector<size_t> intersected = std::vector<size_t>();
            intersected.push_back(t1);
            followSegment(map, tempSegment, intersected);
            differentTrapezoidSamePointP(map, dag, intersected, tempSegment);
        }

    }

    //Same trapezoids
    if(t1==t2 && !samePointP1 && !samePointP2){
        sameTrapezoid(map, tempSegment, t1, dag, map.getTrapezoidByPosition(t1).getNodeId());
    }
    //Different trapezoids
    if(t1!=t2 && !samePointP1 && !samePointP2){
            std::vector<size_t> intersected = std::vector<size_t>();
            intersected.push_back(t1);
            followSegment(map, tempSegment, intersected);
            differentTrapezoid(map, dag, intersected, tempSegment);
        }


}

/**
 * @brief differentTrapezoid Update map and dag when the end point are in different trapezoid
 * @param map Map
 * @param dag Dag
 * @param intersected Intersected trapezoid
 * @param segment Segment
 */
void differentTrapezoid(TrapezoidalMap &map, Dag &dag, const std::vector<size_t> &intersected, const cg3::Segment2d &segment){
    std::vector<size_t> upper = std::vector<size_t>();
    std::vector<size_t> lower = std::vector<size_t>();
    std::vector<size_t> ids = std::vector<size_t>();

    //Adding the segment and points in the map
    size_t segmentId = map.addSegment(segment);
    size_t point1Id = map.addPoint(segment.p1());
    size_t point2Id = map.addPoint(segment.p2());
    ids.push_back(point1Id);
    ids.push_back(point2Id);
    ids.push_back(segmentId);


    addNewTopTrapezoids(map, intersected, upper, point1Id, point2Id, segmentId);
    addNewBottomTrapezoids(map, intersected, lower, point1Id, point2Id, segmentId);

    //Update the first and the last trapezoid
    Trapezoid &tA = map.getTrapezoidByPosition(intersected[0]);
    Trapezoid &tB = map.getTrapezoidByPosition(intersected.back());

    tA.setRight(point1Id);
    tA.setAdjTopRight(upper[0]);
    tA.setAdjBottomRight(lower[0]);

    tB.setLeft(point2Id);
    tB.setAdjTopLeft(upper.back());
    tB.setAdjBottomLeft(lower.back());

    DagAlgorithms::updateDagDifferentTrapezoid(map, dag, intersected, upper, lower, ids);
    deleteIntersectedTrapezoid(map, intersected);

}

/**
 * @brief addNewBottomTrapezoids Add in the map the new trapezoid that are under the new segment
 * @param map Map
 * @param intersected List of intersected trapezoid
 * @param lower List the will contain the position in the map of the new trapezoid
 * @param point1Id The position in the map of the left-end point
 * @param point2Id The position in the map of the right-end point
 * @param segmentId The position in the map of the segment
 */
void addNewBottomTrapezoids(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &lower, const size_t point1Id, const size_t point2Id, const size_t segmentId){
    size_t i = 0;
    size_t start = 0;
    size_t idNewTrapezoid = map.trapezoidVectorSize();
    Trapezoid temp;

    //Creating all the bottom trapezoid (the trapezoid on the bottom of the segment)
    while(i<(intersected.size()-1)){

        //The trapezoid is created only when the adj trapezoid has different bottom
        if(map.getTrapezoidByPosition(intersected[i]).getBottom()!=map.getTrapezoidByPosition(intersected[i+1]).getBottom()){

            temp = Trapezoid();

            //Creating the new trapezoid
            temp.setBottom(map.getTrapezoidByPosition(intersected[i]).getBottom());
            temp.setTop(segmentId);
            temp.setRight(map.getTrapezoidByPosition(intersected[i]).getRight());
            if(start==0){
                temp.setLeft(point1Id);
                temp.setAdjBottomLeft(intersected[0]);
            }else{
                temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
                temp.setAdjBottomLeft(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft());
                temp.setAdjTopLeft(idNewTrapezoid-1);
                map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjTopRight(idNewTrapezoid);
                if(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()!=SIZE_MAX)
                    map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()).setAdjBottomRight(idNewTrapezoid);
            }
            temp.setAdjBottomRight(map.getTrapezoidByPosition(intersected[i]).getAdjBottomRight());
            if(map.getTrapezoidByPosition(intersected[i]).getAdjBottomRight()!=SIZE_MAX)
                map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[i]).getAdjBottomRight()).setAdjBottomLeft(idNewTrapezoid);

            lower.push_back(map.addTrapezoid(temp));
            start=i+1;
            idNewTrapezoid++;
        }
        i++;
    }

    //Same as in the cycle but now we are considering also the last intersected trapezoid, so the rightP is the right end-point
    temp = Trapezoid();
    temp.setBottom(map.getTrapezoidByPosition(intersected[i]).getBottom());
    temp.setTop(segmentId);
    temp.setRight(point2Id);
    if(start==0){
        temp.setLeft(point1Id);
        temp.setAdjBottomLeft(intersected[0]);
    } else{
        temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
        temp.setAdjBottomLeft(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft());
        temp.setAdjTopLeft(idNewTrapezoid-1);
        map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjTopRight(idNewTrapezoid);
        if(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()!=SIZE_MAX)
            map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()).setAdjBottomRight(idNewTrapezoid);

    }
    temp.setAdjBottomRight(intersected.back());

    lower.push_back(map.addTrapezoid(temp));
}

/**
 * @brief addNewTopTrapezoids Add in the map the new trapezoid that are over the new segment
 * @param map Map
 * @param intersected List of intersected trapezoid
 * @param lower List the will contain the position in the map of the new trapezoid
 * @param point1Id The position in the map of the left-end point
 * @param point2Id The position in the map of the right-end point
 * @param segmentId The position in the map of the segment
 */
void addNewTopTrapezoids(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &upper, const size_t point1Id, const size_t point2Id, const size_t segmentId){
    size_t i = 0;
    size_t start = 0;
    size_t idNewTrapezoid = map.trapezoidVectorSize();
    Trapezoid temp;

    //Creating all the top trapezoid (the trapezoid on the top of the segment)
    while(i<(intersected.size()-1)){
        //The trapezoid is created only when the adj trapezoid has different top
        if(map.getTrapezoidByPosition(intersected[i]).getTop()!=map.getTrapezoidByPosition(intersected[i+1]).getTop()){

            temp = Trapezoid();

            //Creating the new trapezoid
            temp.setTop(map.getTrapezoidByPosition(intersected[i]).getTop());
            temp.setBottom(segmentId);
            temp.setRight(map.getTrapezoidByPosition(intersected[i]).getRight());
            if(start==0){
                temp.setLeft(point1Id);
                temp.setAdjTopLeft(intersected[0]);
            }
            else{
                temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
                temp.setAdjTopLeft(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft());
                temp.setAdjBottomLeft(idNewTrapezoid-1);
                map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjBottomRight(idNewTrapezoid);
                if(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()!=SIZE_MAX)
                    map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()).setAdjTopRight(idNewTrapezoid);

            }
            temp.setAdjTopRight(map.getTrapezoidByPosition(intersected[i]).getAdjTopRight());
            if(map.getTrapezoidByPosition(intersected[i]).getAdjTopRight()!=SIZE_MAX)
                map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[i]).getAdjTopRight()).setAdjTopLeft(idNewTrapezoid);


            upper.push_back(map.addTrapezoid(temp));//Adding the new trapezoid in the map and in upper
            start=i+1;
            idNewTrapezoid++;
        }
        i++;
    }

    //Same as in the cycle but now we are considering also the last intersected trapezoid, so the rightP is the right end-point
    temp = Trapezoid();
    temp.setTop(map.getTrapezoidByPosition(intersected[i]).getTop());
    temp.setBottom(segmentId);
    temp.setRight(point2Id);
    if(start==0){
        temp.setLeft(point1Id);
        temp.setAdjTopLeft(intersected[0]);
    }
    else{
        temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
        temp.setAdjTopLeft(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft());
        temp.setAdjBottomLeft(idNewTrapezoid-1);
        map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjBottomRight(idNewTrapezoid);
        if(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()!=SIZE_MAX)
            map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()).setAdjTopRight(idNewTrapezoid);

    }
    temp.setAdjTopRight(intersected.back());

    upper.push_back(map.addTrapezoid(temp));
}



/**
 * @brief differentTrapezoid Update map and dag when the end point are in different trapezoid but the right end-point is already in
 * @param map Map
 * @param dag Dag
 * @param intersected Intersected trapezoid
 * @param segment Segment
 */
void differentTrapezoidSamePointQ(TrapezoidalMap &map, Dag &dag, const std::vector<size_t> &intersected, const cg3::Segment2d &segment){
    std::vector<size_t> upper = std::vector<size_t>();
    std::vector<size_t> lower = std::vector<size_t>();
    std::vector<size_t> ids = std::vector<size_t>();

    //Adding the segment and points in the map
    size_t segmentId = map.addSegment(segment);
    size_t point1Id = map.addPoint(segment.p1());
    size_t point2Id = map.getTrapezoidByPosition(intersected.back()).getRight();
    ids.push_back(point1Id);
    ids.push_back(point2Id);
    ids.push_back(segmentId);


    addNewTopTrapezoidsSamePointQ(map, intersected, upper, point1Id, point2Id, segmentId);
    addNewBottomTrapezoidsSamePointQ(map, intersected, lower, point1Id, point2Id, segmentId);

    //Update the first and the last trapezoid
    Trapezoid &tA = map.getTrapezoidByPosition(intersected[0]);


    tA.setRight(point1Id);
    tA.setAdjTopRight(upper[0]);
    tA.setAdjBottomRight(lower[0]);

    DagAlgorithms::updateDagDifferentTrapezoidSamePointQ(map, dag, intersected, upper, lower, ids);
    deleteIntersectedTrapezoid(map, intersected);
    map.getTrapezoidByPosition(intersected.back()).setDeleted();//Set the delete of the last trapezoid

}

/**
 * @brief addNewBottomTrapezoids Add in the map the new trapezoid that are under the new segment. Version where but the right end-point is already in
 * @param map Map
 * @param intersected List of intersected trapezoid
 * @param lower List the will contain the position in the map of the new trapezoid
 * @param point1Id The position in the map of the left-end point
 * @param point2Id The position in the map of the right-end point
 * @param segmentId The position in the map of the segment
 */
void addNewBottomTrapezoidsSamePointQ(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &lower, const size_t point1Id, const size_t point2Id, const size_t segmentId){
    size_t i = 0;
    size_t start = 0;
    size_t idNewTrapezoid = map.trapezoidVectorSize();
    Trapezoid temp;

    //Creating all the bottom trapezoid (the trapezoid on the bottom of the segment)
    while(i<(intersected.size()-1)){

        //The trapezoid is created only when the adj trapezoid has different bottom
        if(map.getTrapezoidByPosition(intersected[i]).getBottom()!=map.getTrapezoidByPosition(intersected[i+1]).getBottom()){

            temp = Trapezoid();

            //Creating the new trapezoid
            temp.setBottom(map.getTrapezoidByPosition(intersected[i]).getBottom());
            temp.setTop(segmentId);
            temp.setRight(map.getTrapezoidByPosition(intersected[i]).getRight());
            if(start==0){
                temp.setLeft(point1Id);
                temp.setAdjBottomLeft(intersected[0]);
            }else{
                temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
                temp.setAdjBottomLeft(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft());
                temp.setAdjTopLeft(idNewTrapezoid-1);
                map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjTopRight(idNewTrapezoid);
                if(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()!=SIZE_MAX)
                    map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()).setAdjBottomRight(idNewTrapezoid);
            }
            temp.setAdjBottomRight(map.getTrapezoidByPosition(intersected[i]).getAdjBottomRight());
            if(map.getTrapezoidByPosition(intersected[i]).getAdjBottomRight()!=SIZE_MAX)
                map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[i]).getAdjBottomRight()).setAdjBottomLeft(idNewTrapezoid);

            lower.push_back(map.addTrapezoid(temp));
            start=i+1;
            idNewTrapezoid++;
        }
        i++;
    }

    //Same as in the cycle but now we are considering also the last intersected trapezoid, so the rightP is the right end-point
    temp = Trapezoid();
    temp.setBottom(map.getTrapezoidByPosition(intersected[i]).getBottom());
    temp.setTop(segmentId);
    temp.setRight(point2Id);
    if(start==0){
        temp.setLeft(point1Id);
        temp.setAdjBottomLeft(intersected[0]);
    } else{
        temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
        temp.setAdjBottomLeft(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft());
        temp.setAdjTopLeft(idNewTrapezoid-1);
        map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjTopRight(idNewTrapezoid);
        if(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()!=SIZE_MAX)
            map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()).setAdjBottomRight(idNewTrapezoid);

    }
    temp.setAdjBottomRight(map.getTrapezoidByPosition(intersected.back()).getAdjBottomRight());
    if(map.getTrapezoidByPosition(intersected.back()).getAdjBottomRight()!=SIZE_MAX){
        lower.push_back(map.addTrapezoid(temp));
        map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected.back()).getAdjBottomRight()).setAdjBottomLeft(lower.back());
    }else
        lower.push_back(map.addTrapezoid(temp));
}

/**
 * @brief addNewTopTrapezoids Add in the map the new trapezoid that are over the new segment. Version where but the right end-point is already in
 * @param map Map
 * @param intersected List of intersected trapezoid
 * @param lower List the will contain the position in the map of the new trapezoid
 * @param point1Id The position in the map of the left-end point
 * @param point2Id The position in the map of the right-end point
 * @param segmentId The position in the map of the segment
 */
void addNewTopTrapezoidsSamePointQ(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &upper, const size_t point1Id, const size_t point2Id, const size_t segmentId){
    size_t i = 0;
    size_t start = 0;
    size_t idNewTrapezoid = map.trapezoidVectorSize();
    Trapezoid temp;

    //Creating all the top trapezoid (the trapezoid on the top of the segment)
    while(i<(intersected.size()-1)){
        //The trapezoid is created only when the adj trapezoid has different top
        if(map.getTrapezoidByPosition(intersected[i]).getTop()!=map.getTrapezoidByPosition(intersected[i+1]).getTop()){

            temp = Trapezoid();

            //Creating the new trapezoid
            temp.setTop(map.getTrapezoidByPosition(intersected[i]).getTop());
            temp.setBottom(segmentId);
            temp.setRight(map.getTrapezoidByPosition(intersected[i]).getRight());
            if(start==0){
                temp.setLeft(point1Id);
                temp.setAdjTopLeft(intersected[0]);
            }
            else{
                temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
                temp.setAdjTopLeft(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft());
                temp.setAdjBottomLeft(idNewTrapezoid-1);
                map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjBottomRight(idNewTrapezoid);
                if(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()!=SIZE_MAX)
                    map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()).setAdjTopRight(idNewTrapezoid);

            }
            temp.setAdjTopRight(map.getTrapezoidByPosition(intersected[i]).getAdjTopRight());
            if(map.getTrapezoidByPosition(intersected[i]).getAdjTopRight()!=SIZE_MAX)
                map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[i]).getAdjTopRight()).setAdjTopLeft(idNewTrapezoid);


            upper.push_back(map.addTrapezoid(temp));//Adding the new trapezoid in the map and in upper
            start=i+1;
            idNewTrapezoid++;
        }
        i++;
    }

    //Same as in the cycle but now we are considering also the last intersected trapezoid, so the rightP is the right end-point
    temp = Trapezoid();
    temp.setTop(map.getTrapezoidByPosition(intersected[i]).getTop());
    temp.setBottom(segmentId);
    temp.setRight(point2Id);
    if(start==0){
        temp.setLeft(point1Id);
        temp.setAdjTopLeft(intersected[0]);
    }
    else{
        temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
        temp.setAdjTopLeft(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft());
        temp.setAdjBottomLeft(idNewTrapezoid-1);
        map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjBottomRight(idNewTrapezoid);
        if(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()!=SIZE_MAX)
            map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()).setAdjTopRight(idNewTrapezoid);

    }
    temp.setAdjTopRight(map.getTrapezoidByPosition(intersected.back()).getAdjTopRight());
    if(map.getTrapezoidByPosition(intersected.back()).getAdjTopRight()!=SIZE_MAX){
        upper.push_back(map.addTrapezoid(temp));
        map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected.back()).getAdjTopRight()).setAdjTopLeft(upper.back());
    }else
        upper.push_back(map.addTrapezoid(temp));

}

/**
 * @brief differentTrapezoid Update map and dag when the end point are in different trapezoid but the left end-point is already in
 * @param map Map
 * @param dag Dag
 * @param intersected Intersected trapezoid
 * @param segment Segment
 */
void differentTrapezoidSamePointP(TrapezoidalMap &map, Dag &dag, const std::vector<size_t> &intersected, const cg3::Segment2d &segment){
    std::vector<size_t> upper = std::vector<size_t>();
    std::vector<size_t> lower = std::vector<size_t>();
    std::vector<size_t> ids = std::vector<size_t>();

    //Adding the segment and points in the map
    size_t segmentId = map.addSegment(segment);
    size_t point1Id = map.getTrapezoidByPosition(intersected[0]).getLeft();
    size_t point2Id = map.addPoint(segment.p2());
    ids.push_back(point1Id);
    ids.push_back(point2Id);
    ids.push_back(segmentId);


    addNewTopTrapezoidsSamePointP(map, intersected, upper, point1Id, point2Id, segmentId);
    addNewBottomTrapezoidsSamePointP(map, intersected, lower, point1Id, point2Id, segmentId);

    //Update the first and the last trapezoid
    Trapezoid &tB = map.getTrapezoidByPosition(intersected.back());

    tB.setLeft(point2Id);
    tB.setAdjTopLeft(upper.back());
    tB.setAdjBottomLeft(lower.back());

    DagAlgorithms::updateDagDifferentTrapezoidSamePointP(map, dag, intersected, upper, lower, ids);
    deleteIntersectedTrapezoid(map, intersected);
    map.getTrapezoidByPosition(intersected[0]).setDeleted();//Set the delete on the first trapezoid

}

/**
 * @brief addNewBottomTrapezoids Add in the map the new trapezoid that are under the new segment. Version where but the left end-point is already in
 * @param map Map
 * @param intersected List of intersected trapezoid
 * @param lower List the will contain the position in the map of the new trapezoid
 * @param point1Id The position in the map of the left-end point
 * @param point2Id The position in the map of the right-end point
 * @param segmentId The position in the map of the segment
 */
void addNewBottomTrapezoidsSamePointP(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &lower, const size_t point1Id, const size_t point2Id, const size_t segmentId){
    size_t i = 0;
    size_t start = 0;
    size_t idNewTrapezoid = map.trapezoidVectorSize();
    Trapezoid temp;

    //Creating all the bottom trapezoid (the trapezoid on the bottom of the segment)
    while(i<(intersected.size()-1)){

        //The trapezoid is created only when the adj trapezoid has different bottom
        if(map.getTrapezoidByPosition(intersected[i]).getBottom()!=map.getTrapezoidByPosition(intersected[i+1]).getBottom()){

            temp = Trapezoid();

            //Creating the new trapezoid
            temp.setBottom(map.getTrapezoidByPosition(intersected[i]).getBottom());
            temp.setTop(segmentId);
            temp.setRight(map.getTrapezoidByPosition(intersected[i]).getRight());
            if(start==0){
                temp.setLeft(point1Id);

                temp.setAdjBottomLeft(map.getTrapezoidByPosition(intersected[0]).getAdjBottomLeft());
                if(map.getTrapezoidByPosition(intersected[0]).getAdjBottomLeft()!=SIZE_MAX){
                   map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[0]).getAdjBottomLeft()).setAdjBottomRight(idNewTrapezoid);
                }
            }else{
                temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
                temp.setAdjBottomLeft(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft());
                temp.setAdjTopLeft(idNewTrapezoid-1);
                map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjTopRight(idNewTrapezoid);
                if(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()!=SIZE_MAX)
                    map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()).setAdjBottomRight(idNewTrapezoid);
            }
            temp.setAdjBottomRight(map.getTrapezoidByPosition(intersected[i]).getAdjBottomRight());
            if(map.getTrapezoidByPosition(intersected[i]).getAdjBottomRight()!=SIZE_MAX)
                map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[i]).getAdjBottomRight()).setAdjBottomLeft(idNewTrapezoid);

            lower.push_back(map.addTrapezoid(temp));
            start=i+1;
            idNewTrapezoid++;
        }
        i++;
    }

    //Same as in the cycle but now we are considering also the last intersected trapezoid, so the rightP is the right end-point
    temp = Trapezoid();
    temp.setBottom(map.getTrapezoidByPosition(intersected[i]).getBottom());
    temp.setTop(segmentId);
    temp.setRight(point2Id);
    if(start==0){
        temp.setLeft(point1Id);
        temp.setAdjBottomLeft(map.getTrapezoidByPosition(intersected[0]).getAdjBottomLeft());
        if(map.getTrapezoidByPosition(intersected[0]).getAdjBottomLeft()!=SIZE_MAX){
           map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[0]).getAdjBottomLeft()).setAdjBottomRight(idNewTrapezoid);
        }
    } else{
        temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
        temp.setAdjBottomLeft(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft());
        temp.setAdjTopLeft(idNewTrapezoid-1);
        map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjTopRight(idNewTrapezoid);
        if(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()!=SIZE_MAX)
            map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()).setAdjBottomRight(idNewTrapezoid);

    }
    temp.setAdjBottomRight(intersected.back());

    lower.push_back(map.addTrapezoid(temp));
}

/**
 * @brief addNewTopTrapezoids Add in the map the new trapezoid that are over the new segment. Version where but the left end-point is already in
 * @param map Map
 * @param intersected List of intersected trapezoid
 * @param lower List the will contain the position in the map of the new trapezoid
 * @param point1Id The position in the map of the left-end point
 * @param point2Id The position in the map of the right-end point
 * @param segmentId The position in the map of the segment
 */
void addNewTopTrapezoidsSamePointP(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &upper, const size_t point1Id, const size_t point2Id, const size_t segmentId){
    size_t i = 0;
    size_t start = 0;
    size_t idNewTrapezoid = map.trapezoidVectorSize();
    Trapezoid temp;

    //Creating all the top trapezoid (the trapezoid on the top of the segment)
    while(i<(intersected.size()-1)){
        //The trapezoid is created only when the adj trapezoid has different top
        if(map.getTrapezoidByPosition(intersected[i]).getTop()!=map.getTrapezoidByPosition(intersected[i+1]).getTop()){

            temp = Trapezoid();

            //Creating the new trapezoid
            temp.setTop(map.getTrapezoidByPosition(intersected[i]).getTop());
            temp.setBottom(segmentId);
            temp.setRight(map.getTrapezoidByPosition(intersected[i]).getRight());
            if(start==0){
                temp.setLeft(point1Id);
                temp.setAdjTopLeft(map.getTrapezoidByPosition(intersected[0]).getAdjTopLeft());
                if(map.getTrapezoidByPosition(intersected[0]).getAdjTopLeft()!=SIZE_MAX){
                   map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[0]).getAdjTopLeft()).setAdjTopRight(idNewTrapezoid);
                }
            }
            else{
                temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
                temp.setAdjTopLeft(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft());
                temp.setAdjBottomLeft(idNewTrapezoid-1);
                map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjBottomRight(idNewTrapezoid);
                if(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()!=SIZE_MAX)
                    map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()).setAdjTopRight(idNewTrapezoid);

            }
            temp.setAdjTopRight(map.getTrapezoidByPosition(intersected[i]).getAdjTopRight());
            if(map.getTrapezoidByPosition(intersected[i]).getAdjTopRight()!=SIZE_MAX)
                map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[i]).getAdjTopRight()).setAdjTopLeft(idNewTrapezoid);


            upper.push_back(map.addTrapezoid(temp));//Adding the new trapezoid in the map and in upper
            start=i+1;
            idNewTrapezoid++;
        }
        i++;
    }

    //Same as in the cycle but now we are considering also the last intersected trapezoid, so the rightP is the right end-point
    temp = Trapezoid();
    temp.setTop(map.getTrapezoidByPosition(intersected[i]).getTop());
    temp.setBottom(segmentId);
    temp.setRight(point2Id);
    if(start==0){
        temp.setLeft(point1Id);
        temp.setAdjTopLeft(map.getTrapezoidByPosition(intersected[0]).getAdjTopLeft());
        if(map.getTrapezoidByPosition(intersected[0]).getAdjTopLeft()!=SIZE_MAX){
           map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[0]).getAdjTopLeft()).setAdjTopRight(idNewTrapezoid);
        }
    }
    else{
        temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
        temp.setAdjTopLeft(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft());
        temp.setAdjBottomLeft(idNewTrapezoid-1);
        map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjBottomRight(idNewTrapezoid);
        if(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()!=SIZE_MAX)
            map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()).setAdjTopRight(idNewTrapezoid);

    }
    temp.setAdjTopRight(intersected.back());
    map.getTrapezoidByPosition(intersected.back()).setAdjTopLeft(idNewTrapezoid);

    upper.push_back(map.addTrapezoid(temp));
}

/**
 * @brief Delete the intersected trapezoid from the map (not the first and the last)
 * @param map
 * @param intersected
 */
void deleteIntersectedTrapezoid(TrapezoidalMap & map, const std::vector<size_t> &intersected){
    size_t i = 1;
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
void sameTrapezoid(TrapezoidalMap &map, const cg3::Segment2d &segment, const size_t trapezoidAId, Dag & dag, const size_t nodeId){


    std::vector<size_t> idVector = std::vector<size_t>();
    Trapezoid t = map.getTrapezoidByPosition(trapezoidAId);

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

    if(trapezoidA.getAdjTopRight()!=SIZE_MAX){
        map.getTrapezoidByPosition(trapezoidA.getAdjTopRight()).setAdjTopLeft(trapezoidDId);
    }

    if(trapezoidA.getAdjBottomRight()!=SIZE_MAX){
        map.getTrapezoidByPosition(trapezoidA.getAdjBottomRight()).setAdjBottomLeft(trapezoidDId);
    }

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
    DagAlgorithms::updateDagSameTrapezoid(map, dag, nodeId, idVector);
}

/**
 * @brief sameTrapezoid Update the trapezoidal map and the dag when the end-points of the segment are in the same trapezoid and the right end-point is the same as another point
 * @param map The map
 * @param segment The segment
 * @param trapezoidAId The position of the trapezoid in the map
 * @param dag The dag
 * @param nodeId The position of the node in the dag
 */
void sameTrapezoidSamePointQ(TrapezoidalMap &map, const cg3::Segment2d &segment, const size_t trapezoidAId, Dag & dag, const size_t nodeId){

    std::vector<size_t> idVector = std::vector<size_t>();
    Trapezoid t = map.getTrapezoidByPosition(trapezoidAId);

    //Adding the segment, points and trapezoids in the map
    size_t segmentId = map.addSegment(segment);
    size_t point1Id = map.addPoint(segment.p1());
    size_t point2Id = map.getTrapezoidByPosition(trapezoidAId).getRight();

    size_t trapezoidBId = map.addTrapezoid(Trapezoid(point1Id, point2Id, t.getTop(), segmentId));
    size_t trapezoidCId = map.addTrapezoid(Trapezoid(point1Id, point2Id, segmentId, t.getBottom()));


    idVector.push_back(segmentId);
    idVector.push_back(point1Id);
    idVector.push_back(point2Id);
    idVector.push_back(trapezoidAId);
    idVector.push_back(trapezoidBId);
    idVector.push_back(trapezoidCId);

    //Manipulating the trapezoids
    Trapezoid & trapezoidA = map.getTrapezoidByPosition(trapezoidAId);
    Trapezoid & trapezoidB = map.getTrapezoidByPosition(trapezoidBId);
    Trapezoid & trapezoidC = map.getTrapezoidByPosition(trapezoidCId);

    if(trapezoidA.getAdjTopRight()!=SIZE_MAX){
        map.getTrapezoidByPosition(trapezoidA.getAdjTopRight()).setAdjTopLeft(trapezoidBId);
    }

    if(trapezoidA.getAdjBottomRight()!=SIZE_MAX){
        map.getTrapezoidByPosition(trapezoidA.getAdjBottomRight()).setAdjBottomLeft(trapezoidCId);
    }

    trapezoidB.setAdjTopLeft(trapezoidAId);
    trapezoidB.setAdjBottomLeft(SIZE_MAX);
    trapezoidB.setAdjTopRight(trapezoidA.getAdjTopRight());
    trapezoidB.setAdjBottomRight(SIZE_MAX);

    trapezoidC.setAdjTopLeft(SIZE_MAX);
    trapezoidC.setAdjBottomLeft(trapezoidAId);
    trapezoidC.setAdjTopRight(SIZE_MAX);
    trapezoidC.setAdjBottomRight(trapezoidA.getAdjBottomRight());

    trapezoidA.setRight(point1Id);
    trapezoidA.setAdjTopRight(trapezoidBId);
    trapezoidA.setAdjBottomRight(trapezoidCId);

    //Update dag
    DagAlgorithms::updateDagSameTrapezoidSamePointQ(map, dag, nodeId, idVector);
}

/**
 * @brief sameTrapezoid Update the trapezoidal map and the dag when the end-points of the segment are in the same trapezoid and the left end-point is the same as another point
 * @param map The map
 * @param segment The segment
 * @param trapezoidAId The position of the trapezoid in the map
 * @param dag The dag
 * @param nodeId The position of the node in the dag
 */
void sameTrapezoidSamePointP(TrapezoidalMap &map, const cg3::Segment2d &segment, const size_t trapezoidAId, Dag & dag, const size_t nodeId){

    std::vector<size_t> idVector = std::vector<size_t>();
    Trapezoid t = map.getTrapezoidByPosition(trapezoidAId);

    //Adding the segment, points and trapezoids in the map
    size_t segmentId = map.addSegment(segment);
    size_t point1Id = map.getTrapezoidByPosition(trapezoidAId).getLeft();
    size_t point2Id = map.addPoint(segment.p2());

    size_t trapezoidBId = map.addTrapezoid(Trapezoid(point1Id, point2Id, t.getTop(), segmentId));
    size_t trapezoidCId = map.addTrapezoid(Trapezoid(point1Id, point2Id, segmentId, t.getBottom()));


    idVector.push_back(segmentId);
    idVector.push_back(point1Id);
    idVector.push_back(point2Id);
    idVector.push_back(trapezoidAId);
    idVector.push_back(trapezoidBId);
    idVector.push_back(trapezoidCId);

    //Manipulating the trapezoids
    Trapezoid & trapezoidA = map.getTrapezoidByPosition(trapezoidAId);
    Trapezoid & trapezoidB = map.getTrapezoidByPosition(trapezoidBId);
    Trapezoid & trapezoidC = map.getTrapezoidByPosition(trapezoidCId);

    if(trapezoidA.getAdjTopLeft()!=SIZE_MAX){
        map.getTrapezoidByPosition(trapezoidA.getAdjTopLeft()).setAdjTopRight(trapezoidBId);
    }

    if(trapezoidA.getAdjBottomLeft()!=SIZE_MAX){
        map.getTrapezoidByPosition(trapezoidA.getAdjBottomLeft()).setAdjBottomRight(trapezoidCId);
    }


    trapezoidB.setAdjTopLeft(trapezoidA.getAdjTopLeft());
    trapezoidB.setAdjBottomLeft(SIZE_MAX);
    trapezoidB.setAdjTopRight(trapezoidAId);
    trapezoidB.setAdjBottomRight(SIZE_MAX);

    trapezoidC.setAdjTopLeft(SIZE_MAX);
    trapezoidC.setAdjBottomLeft(trapezoidA.getAdjBottomLeft());
    trapezoidC.setAdjTopRight(SIZE_MAX);
    trapezoidC.setAdjBottomRight(trapezoidAId);

    trapezoidA.setLeft(point2Id);
    trapezoidA.setAdjTopLeft(trapezoidBId);
    trapezoidA.setAdjBottomLeft(trapezoidCId);

    //Update dag
    DagAlgorithms::updateDagSameTrapezoidSamePointP(map, dag, nodeId, idVector);
}

/**
 * @brief sameTrapezoid Update the trapezoidal map and the dag when the end-points of the segment are in the same trapezoid and th end-points are already in
 * @param map The map
 * @param segment The segment
 * @param trapezoidAId The position of the trapezoid in the map
 * @param dag The dag
 * @param nodeId The position of the node in the dag
 */
void sameTrapezoidSamePointPQ(TrapezoidalMap &map, const cg3::Segment2d &segment, const size_t trapezoidAId, Dag & dag, const size_t nodeId){

    std::vector<size_t> idVector = std::vector<size_t>();
    Trapezoid t = map.getTrapezoidByPosition(trapezoidAId);

    //Adding the segment, points and trapezoids in the map
    size_t segmentId = map.addSegment(segment);
    size_t point1Id = map.getTrapezoidByPosition(trapezoidAId).getLeft();
    size_t point2Id = map.getTrapezoidByPosition(trapezoidAId).getRight();

    size_t trapezoidBId = map.addTrapezoid(Trapezoid(point1Id, point2Id, segmentId, t.getBottom()));


    idVector.push_back(segmentId);
    idVector.push_back(point1Id);
    idVector.push_back(point2Id);
    idVector.push_back(trapezoidAId);
    idVector.push_back(trapezoidBId);

    //Manipulating the trapezoids
    Trapezoid & trapezoidA = map.getTrapezoidByPosition(trapezoidAId);
    Trapezoid & trapezoidB = map.getTrapezoidByPosition(trapezoidBId);

    if(trapezoidA.getAdjBottomLeft()!=SIZE_MAX){
        map.getTrapezoidByPosition(trapezoidA.getAdjBottomLeft()).setAdjBottomRight(trapezoidBId);
    }

    if(trapezoidA.getAdjBottomRight()!=SIZE_MAX){
        map.getTrapezoidByPosition(trapezoidA.getAdjBottomRight()).setAdjBottomLeft(trapezoidBId);
    }


    trapezoidB.setAdjTopLeft(SIZE_MAX);
    trapezoidB.setAdjBottomLeft(trapezoidA.getAdjBottomLeft());
    trapezoidB.setAdjTopRight(SIZE_MAX);
    trapezoidB.setAdjBottomRight(trapezoidA.getAdjBottomRight());

    trapezoidA.setAdjBottomLeft(SIZE_MAX);
    trapezoidA.setAdjBottomRight(SIZE_MAX);
    trapezoidA.setBottom(segmentId);

    //Update dag
    DagAlgorithms::updateDagSameTrapezoidSamePointPQ(map, dag, nodeId, idVector);
}

/**
 * @brief differentTrapezoid Update map and dag when the end point are in different trapezoid and the end-points are already in
 * @param map Map
 * @param dag Dag
 * @param intersected Intersected trapezoid
 * @param segment Segment
 */
void differentTrapezoidSamePointPQ(TrapezoidalMap &map, Dag &dag, const std::vector<size_t> &intersected, const cg3::Segment2d &segment){
    std::vector<size_t> upper = std::vector<size_t>();
    std::vector<size_t> lower = std::vector<size_t>();
    std::vector<size_t> ids = std::vector<size_t>();

    //Adding the segment and points in the map
    size_t segmentId = map.addSegment(segment);
    size_t point1Id = map.getTrapezoidByPosition(intersected[0]).getLeft();
    size_t point2Id = map.getTrapezoidByPosition(intersected.back()).getRight();
    ids.push_back(point1Id);
    ids.push_back(point2Id);
    ids.push_back(segmentId);


    addNewTopTrapezoidsSamePointPQ(map, intersected, upper, point1Id, point2Id, segmentId);
    addNewBottomTrapezoidsSamePointPQ(map, intersected, lower, point1Id, point2Id, segmentId);

    DagAlgorithms::updateDagDifferentTrapezoidSamePointPQ(map, dag, intersected, upper, lower, ids);

    deleteIntersectedTrapezoid(map, intersected);
    map.getTrapezoidByPosition(intersected[0]).setDeleted();//Set delete on the first trapezoid
    map.getTrapezoidByPosition(intersected.back()).setDeleted();//Set delete on the last trapezoid

}

/**
 * @brief addNewBottomTrapezoids Add in the map the new trapezoid that are under the new segment when the end point are in different trapezoid and the end-points are already in
 * @param map Map
 * @param intersected List of intersected trapezoid
 * @param lower List the will contain the position in the map of the new trapezoid
 * @param point1Id The position in the map of the left-end point
 * @param point2Id The position in the map of the right-end point
 * @param segmentId The position in the map of the segment
 */
void addNewBottomTrapezoidsSamePointPQ(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &lower, const size_t point1Id, const size_t point2Id, const size_t segmentId){
    size_t i = 0;
    size_t start = 0;
    size_t idNewTrapezoid = map.trapezoidVectorSize();
    Trapezoid temp;

    //Creating all the bottom trapezoid (the trapezoid on the bottom of the segment)
    while(i<(intersected.size()-1)){

        //The trapezoid is created only when the adj trapezoid has different bottom
        if(map.getTrapezoidByPosition(intersected[i]).getBottom()!=map.getTrapezoidByPosition(intersected[i+1]).getBottom()){

            temp = Trapezoid();

            //Creating the new trapezoid
            temp.setBottom(map.getTrapezoidByPosition(intersected[i]).getBottom());
            temp.setTop(segmentId);
            temp.setRight(map.getTrapezoidByPosition(intersected[i]).getRight());
            if(start==0){
               temp.setLeft(point1Id);

               temp.setAdjBottomLeft(map.getTrapezoidByPosition(intersected[0]).getAdjBottomLeft());
               if(map.getTrapezoidByPosition(intersected[0]).getAdjBottomLeft()!=SIZE_MAX){
                  map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[0]).getAdjBottomLeft()).setAdjBottomRight(idNewTrapezoid);
               }
            }else{
                temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
                temp.setAdjBottomLeft(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft());
                temp.setAdjTopLeft(idNewTrapezoid-1);
                map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjTopRight(idNewTrapezoid);
                if(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()!=SIZE_MAX)
                    map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()).setAdjBottomRight(idNewTrapezoid);
            }
            temp.setAdjBottomRight(map.getTrapezoidByPosition(intersected[i]).getAdjBottomRight());
            if(map.getTrapezoidByPosition(intersected[i]).getAdjBottomRight()!=SIZE_MAX)
                map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[i]).getAdjBottomRight()).setAdjBottomLeft(idNewTrapezoid);

            lower.push_back(map.addTrapezoid(temp));
            start=i+1;
            idNewTrapezoid++;
        }
        i++;
    }

    //Same as in the cycle but now we are considering also the last intersected trapezoid, so the rightP is the right end-point
    temp = Trapezoid();
    temp.setBottom(map.getTrapezoidByPosition(intersected[i]).getBottom());
    temp.setTop(segmentId);
    temp.setRight(point2Id);
    if(start==0){
        temp.setLeft(point1Id);
        temp.setAdjBottomLeft(map.getTrapezoidByPosition(intersected[0]).getAdjBottomLeft());
        if(map.getTrapezoidByPosition(intersected[0]).getAdjBottomLeft()!=SIZE_MAX){
           map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[0]).getAdjBottomLeft()).setAdjBottomRight(idNewTrapezoid);
        }
    } else{
        temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
        temp.setAdjBottomLeft(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft());
        temp.setAdjTopLeft(idNewTrapezoid-1);
        map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjTopRight(idNewTrapezoid);
        if(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()!=SIZE_MAX)
            map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjBottomLeft()).setAdjBottomRight(idNewTrapezoid);

    }
    temp.setAdjBottomRight(map.getTrapezoidByPosition(intersected.back()).getAdjBottomRight());
    if(map.getTrapezoidByPosition(intersected.back()).getAdjBottomRight()!=SIZE_MAX){
        lower.push_back(map.addTrapezoid(temp));
        map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected.back()).getAdjBottomRight()).setAdjBottomLeft(lower.back());
    }else
        lower.push_back(map.addTrapezoid(temp));
}

/**
 * @brief addNewTopTrapezoids Add in the map the new trapezoid that are over the new segment when the end point are in different trapezoid and the end-points are already in
 * @param map Map
 * @param intersected List of intersected trapezoid
 * @param lower List the will contain the position in the map of the new trapezoid
 * @param point1Id The position in the map of the left-end point
 * @param point2Id The position in the map of the right-end point
 * @param segmentId The position in the map of the segment
 */
void addNewTopTrapezoidsSamePointPQ(TrapezoidalMap & map, const std::vector<size_t> &intersected, std::vector<size_t> &upper, const size_t point1Id, const size_t point2Id, const size_t segmentId){
    size_t i = 0;
    size_t start = 0;
    size_t idNewTrapezoid = map.trapezoidVectorSize();
    Trapezoid temp;

    //Creating all the top trapezoid (the trapezoid on the top of the segment)
    while(i<(intersected.size()-1)){
        //The trapezoid is created only when the adj trapezoid has different top
        if(map.getTrapezoidByPosition(intersected[i]).getTop()!=map.getTrapezoidByPosition(intersected[i+1]).getTop()){

            temp = Trapezoid();

            //Creating the new trapezoid
            temp.setTop(map.getTrapezoidByPosition(intersected[i]).getTop());
            temp.setBottom(segmentId);
            temp.setRight(map.getTrapezoidByPosition(intersected[i]).getRight());
            if(start==0){
                temp.setLeft(point1Id);
                temp.setAdjTopLeft(map.getTrapezoidByPosition(intersected[0]).getAdjTopLeft());
                if(map.getTrapezoidByPosition(intersected[0]).getAdjTopLeft()!=SIZE_MAX){
                   map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[0]).getAdjTopLeft()).setAdjTopRight(idNewTrapezoid);
                }
            }
            else{
                temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
                temp.setAdjTopLeft(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft());
                temp.setAdjBottomLeft(idNewTrapezoid-1);
                map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjBottomRight(idNewTrapezoid);
                if(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()!=SIZE_MAX)
                    map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()).setAdjTopRight(idNewTrapezoid);

            }
            temp.setAdjTopRight(map.getTrapezoidByPosition(intersected[i]).getAdjTopRight());
            if(map.getTrapezoidByPosition(intersected[i]).getAdjTopRight()!=SIZE_MAX)
                map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[i]).getAdjTopRight()).setAdjTopLeft(idNewTrapezoid);


            upper.push_back(map.addTrapezoid(temp));//Adding the new trapezoid in the map and in upper
            start=i+1;
            idNewTrapezoid++;
        }
        i++;
    }

    //Same as in the cycle but now we are considering also the last intersected trapezoid, so the rightP is the right end-point
    temp = Trapezoid();
    temp.setTop(map.getTrapezoidByPosition(intersected[i]).getTop());
    temp.setBottom(segmentId);
    temp.setRight(point2Id);
    if(start==0){
       temp.setLeft(point1Id);
       temp.setAdjTopLeft(map.getTrapezoidByPosition(intersected[0]).getAdjTopLeft());
       if(map.getTrapezoidByPosition(intersected[0]).getAdjTopLeft()!=SIZE_MAX){
          map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[0]).getAdjTopLeft()).setAdjTopRight(idNewTrapezoid);
       }
    }
    else{
            temp.setLeft(map.getTrapezoidByPosition(intersected[start]).getLeft());
            temp.setAdjTopLeft(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft());
            temp.setAdjBottomLeft(idNewTrapezoid-1);
            map.getTrapezoidByPosition(idNewTrapezoid-1).setAdjBottomRight(idNewTrapezoid);
            if(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()!=SIZE_MAX)
                map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected[start]).getAdjTopLeft()).setAdjTopRight(idNewTrapezoid);

        }
        temp.setAdjTopRight(map.getTrapezoidByPosition(intersected.back()).getAdjTopRight());
        if(map.getTrapezoidByPosition(intersected.back()).getAdjTopRight()!=SIZE_MAX){
            upper.push_back(map.addTrapezoid(temp));
            map.getTrapezoidByPosition(map.getTrapezoidByPosition(intersected.back()).getAdjTopRight()).setAdjTopLeft(upper.back());
        }else
            upper.push_back(map.addTrapezoid(temp));
}


/**
 * @brief findRealLeft Returns the true trapezoid to which a point belongs (used when the searched point is the same left point of more than 1 trapezoid)
 * @param map Map
 * @param point The position the the dag of the search point
 * @param segment The segment
 * @return The true trapezoid to which the point belongs
 */
size_t findRealLeft(TrapezoidalMap & map, const size_t point, const cg3::Segment2d &segment){

    std::vector<size_t> trapezoidCandidates = std::vector<size_t>();
    size_t i =0;

    while(i<map.trapezoidVectorSize()){
        if(!map.getTrapezoidByPosition(i).isDeleted()){
            if(map.getTrapezoidByPosition(i).getLeft()==point)
                trapezoidCandidates.push_back(i);
        }
        i++;
    }

    //If there is only one,it is that
    if(trapezoidCandidates.size()==1)
        return trapezoidCandidates[0];

    for(size_t i : trapezoidCandidates){

        bool tP1=AlgorithmsUtils::pointEssentiallyEqual(map.getSegmentByPosition(map.getTrapezoidByPosition(i).getTop()).p1(), segment.p1());
        bool bP1=AlgorithmsUtils::pointEssentiallyEqual(map.getSegmentByPosition(map.getTrapezoidByPosition(i).getBottom()).p1(), segment.p1());

        if(bP1 && tP1){
            //The segment should be between the top and the bottom
            if(!AlgorithmsUtils::isPointOnTheLeft(segment.p1(), segment.p2(), map.getSegmentByPosition(map.getTrapezoidByPosition(i).getBottom()).p2()) &&
                AlgorithmsUtils::isPointOnTheLeft(segment.p1(), segment.p2(), map.getSegmentByPosition(map.getTrapezoidByPosition(i).getTop()).p2()))
                return i;
        }else
            if(bP1){//The segment should be between above bottom
                if(!AlgorithmsUtils::isPointOnTheLeft(segment.p1(), segment.p2(), map.getSegmentByPosition(map.getTrapezoidByPosition(i).getBottom()).p2()))
                    return i;
            }else{//The segment shoul be under the top
                if(AlgorithmsUtils::isPointOnTheLeft(segment.p1(), segment.p2(), map.getSegmentByPosition(map.getTrapezoidByPosition(i).getTop()).p2()))
                    return i;
            }
    }

    return SIZE_MAX;
}


/**
 * @brief followSegment Return a list of trapezoid intersected by the segment s.
 * @param map The map
 * @param segment The segment
 * @param intersected The vector (it must contain the starting trapezoid) that will contain the trapezoids
 */
void followSegment(TrapezoidalMap &map, const cg3::Segment2d segment, std::vector<size_t>& intersected){
    size_t i = 0;

    while(segment.p2().x()>map.getPointByPosition(map.getTrapezoidByPosition(intersected[i]).getRight()).x()){
       if(AlgorithmsUtils::isPointOnTheLeft(segment.p1(), segment.p2(), map.getPointByPosition(map.getTrapezoidByPosition(intersected[i]).getRight()))){
            intersected.push_back(map.getTrapezoidByPosition(intersected[i]).getAdjBottomRight());
        }else{
            intersected.push_back(map.getTrapezoidByPosition(intersected[i]).getAdjTopRight());
        }
        i++;

    }
}

}
