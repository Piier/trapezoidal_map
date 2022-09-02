#ifndef DAG_ALGORITHMS_H
#define DAG_ALGORITHMS_H
#include "algorithms_utils.h"

namespace DagAlgorithms {
    void updateDagSameTrapezoid(TrapezoidalMap &map, Dag &dag, size_t nodeId, std::vector<size_t> & idVector);
    size_t queryDag(TrapezoidalMap &map, Dag &dag, Node &node, const cg3::Point2d & point);
}

#endif // DAG_ALGORITHMS_H
