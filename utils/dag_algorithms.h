#ifndef DAG_ALGORITHMS_H
#define DAG_ALGORITHMS_H
#include "data_structures/trapezoidalmap.h"
#include "data_structures/dag.h"

namespace DagAlgorithms {
    void updateDagSameTrapezoid(Dag &dag, size_t nodeId, std::vector<size_t> & idVector);
}

#endif // DAG_ALGORITHMS_H
