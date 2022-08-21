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
    void addNode(const Node & newNode);

};



#endif // DAG_H
