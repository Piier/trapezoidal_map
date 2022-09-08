#include "map_algorithms.h"

namespace MapAlgorithms {

void addSegmentToMap(TrapezoidalMap &map, Dag &dag, const cg3::Segment2d &segment){
    cg3::Segment2d tempSegment = cg3::Segment2d();

    AlgorithmsUtils::checkSegment(segment, tempSegment);

    size_t t1 = DagAlgorithms::queryDag(map, dag, dag.getRoot(), tempSegment.p1());
    size_t t2 = DagAlgorithms::queryDag(map, dag, dag.getRoot(), tempSegment.p2());
    bool samePointP1=AlgorithmsUtils::pointEssentiallyEqual((map.getPointByPosition(map.getTrapezoidByPosition(t1).getRight())),tempSegment.p1());
    bool samePointP2=AlgorithmsUtils::pointEssentiallyEqual((map.getPointByPosition(map.getTrapezoidByPosition(t2).getRight())),tempSegment.p2());


    //Right end-point is the same as another point
    if(samePointP2){
        //Same trapezoid
        if(t1==t2 || AlgorithmsUtils::pointEssentiallyEqual((map.getPointByPosition(map.getTrapezoidByPosition(t1).getRight())),tempSegment.p2())){
            sameTrapezoidSamePointQ(map, tempSegment, t1, dag, map.getTrapezoidByPosition(t1).getNodeId());
        }else{
            std::vector<size_t> intersected = std::vector<size_t>();
            intersected.push_back(t1);
            AlgorithmsUtils::followSegment(map, tempSegment, intersected);
            differentTrapezoidSamePointQ(map, dag, intersected, tempSegment);
        }
    }

    //Left end-point is the same as another point
    if(samePointP1){
        //Same trapezoid
        if(map.getTrapezoidByPosition(t1).getRight()==map.getTrapezoidByPosition(t2).getLeft()){
            sameTrapezoidSamePointP(map, tempSegment, t2, dag, map.getTrapezoidByPosition(t2).getNodeId());
        }else{//Different trapezoid

            std::vector<size_t> intersected = std::vector<size_t>();
            if(map.getTrapezoidByPosition(t1).getAdjTopRight()==SIZE_MAX){
                intersected.push_back(map.getTrapezoidByPosition(t1).getAdjBottomRight());
            }else{
                //Segmet that start from left-end point
                cg3::Segment2d &divider = map.getSegmentByPosition(map.getTrapezoidByPosition(map.getTrapezoidByPosition(t1).getAdjTopRight()).getBottom());

                if(AlgorithmsUtils::isPointOnTheLeft(divider.p1(), divider.p2(), tempSegment.p2())){
                    intersected.push_back(map.getTrapezoidByPosition(t1).getAdjTopRight());
                }else{
                    intersected.push_back(map.getTrapezoidByPosition(t1).getAdjBottomRight());
                }
            }
            AlgorithmsUtils::followSegment(map, tempSegment, intersected);
            differentTrapezoidSamePointP(map, dag, intersected, tempSegment);
        }

    }

    if(t1==t2 && !samePointP1 && !samePointP2){
        sameTrapezoid(map, tempSegment, t1, dag, map.getTrapezoidByPosition(t1).getNodeId());
    }
    if(t1!=t2 && !samePointP1 && !samePointP2){
            std::vector<size_t> intersected = std::vector<size_t>();
            intersected.push_back(t1);
            AlgorithmsUtils::followSegment(map, tempSegment, intersected);
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
void differentTrapezoid(TrapezoidalMap &map, Dag &dag, std::vector<size_t> &intersected, const cg3::Segment2d &segment){
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
void addNewBottomTrapezoids(TrapezoidalMap & map, std::vector<size_t> &intersected, std::vector<size_t> &lower, size_t point1Id, size_t point2Id, size_t segmentId){
    size_t i = 0;
    size_t start = 0;
    size_t idNewTrapezoid = map.trapezoidVectorSize();
    Trapezoid temp;

    //Creating all the top trapezoid (the trapezoid on the top of the segment)
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
void addNewTopTrapezoids(TrapezoidalMap & map, std::vector<size_t> &intersected, std::vector<size_t> &upper, size_t point1Id, size_t point2Id, size_t segmentId){
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
void differentTrapezoidSamePointQ(TrapezoidalMap &map, Dag &dag, std::vector<size_t> &intersected, const cg3::Segment2d &segment){
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
void addNewBottomTrapezoidsSamePointQ(TrapezoidalMap & map, std::vector<size_t> &intersected, std::vector<size_t> &lower, size_t point1Id, size_t point2Id, size_t segmentId){
    size_t i = 0;
    size_t start = 0;
    size_t idNewTrapezoid = map.trapezoidVectorSize();
    Trapezoid temp;

    //Creating all the top trapezoid (the trapezoid on the top of the segment)
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
void addNewTopTrapezoidsSamePointQ(TrapezoidalMap & map, std::vector<size_t> &intersected, std::vector<size_t> &upper, size_t point1Id, size_t point2Id, size_t segmentId){
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
void differentTrapezoidSamePointP(TrapezoidalMap &map, Dag &dag, std::vector<size_t> &intersected, const cg3::Segment2d &segment){
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
void addNewBottomTrapezoidsSamePointP(TrapezoidalMap & map, std::vector<size_t> &intersected, std::vector<size_t> &lower, size_t point1Id, size_t point2Id, size_t segmentId){
    size_t i = 0;
    size_t start = 0;
    size_t idNewTrapezoid = map.trapezoidVectorSize();
    Trapezoid temp;

    //Creating all the top trapezoid (the trapezoid on the top of the segment)
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
void addNewTopTrapezoidsSamePointP(TrapezoidalMap & map, std::vector<size_t> &intersected, std::vector<size_t> &upper, size_t point1Id, size_t point2Id, size_t segmentId){
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
void deleteIntersectedTrapezoid(TrapezoidalMap & map, std::vector<size_t> &intersected){
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
void sameTrapezoid(TrapezoidalMap &map, const cg3::Segment2d &segment, size_t trapezoidAId, Dag & dag, size_t nodeId){


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
void sameTrapezoidSamePointQ(TrapezoidalMap &map, const cg3::Segment2d &segment, size_t trapezoidAId, Dag & dag, size_t nodeId){

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
void sameTrapezoidSamePointP(TrapezoidalMap &map, const cg3::Segment2d &segment, size_t trapezoidAId, Dag & dag, size_t nodeId){

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

}
