#include "algorithms.h"

namespace Algorithms {

void addSegmentToMap(TrapezoidalMap &map, Dag &dag, const cg3::Segment2d &segment){
    map.addSegment(segment);
    map.addPoint(segment.p1());
    map.addPoint(segment.p2());

    std::cout<<segment.p1()<<"  "<<segment.p2();

}

}
