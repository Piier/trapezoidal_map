#ifndef DAG_ALGORITHMS_H
#define DAG_ALGORITHMS_H
#include "algorithms_utils.h"

namespace DagAlgorithms {
    void updateDagSameTrapezoid(TrapezoidalMap &map, Dag &dag, size_t nodeId, std::vector<size_t> & idVector);
    void updateDagSameTrapezoidSamePointQ(TrapezoidalMap & map, Dag &dag, size_t nodeId, std::vector<size_t> & idVector);
    void updateDagSameTrapezoidSamePointP(TrapezoidalMap & map, Dag &dag, size_t nodeId, std::vector<size_t> & idVector);
    void updateDagSameTrapezoidSamePointPQ(TrapezoidalMap & map, Dag &dag, size_t nodeId, std::vector<size_t> & idVector);

    void updateDagDifferentTrapezoid(TrapezoidalMap &map, Dag &dag, std::vector<size_t> & intersected, std::vector<size_t> &upper, std::vector<size_t> &lower, std::vector<size_t> &ids);
    void updateDagDifferentTrapezoidSamePointQ(TrapezoidalMap &map, Dag &dag, std::vector<size_t> & intersected, std::vector<size_t> &upper, std::vector<size_t> &lower, std::vector<size_t> &ids);
    void updateDagDifferentTrapezoidSamePointP(TrapezoidalMap &map, Dag &dag, std::vector<size_t> & intersected, std::vector<size_t> &upper, std::vector<size_t> &lower, std::vector<size_t> &ids);

    size_t queryDag(TrapezoidalMap &map, Dag &dag, Node &node, const cg3::Point2d & point);
}

#endif // DAG_ALGORITHMS_H
