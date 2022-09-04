#ifndef DAG_H
#define DAG_H
#include "node.h"

class Dag{

private:
    std::vector<Node> dag;


public:
    Dag();

    Node& getNodeByPosition(const size_t position);
    Node& getRoot();
    size_t addNode(Node &newNode);
    size_t getSize();
    void clear();

};



#endif // DAG_H
