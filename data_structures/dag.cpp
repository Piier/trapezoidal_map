#include "dag.h"

/**
 * @brief Dag::Dag Constructor
 */
Dag::Dag(){
    dag=std::vector<Node>();
    dag.push_back(Node(0,Type::T_Trapezoid));
}

/**
 * @brief Dag::getNodeByPosition Get a node reference by is position
 * @param position The position of the node in the dag
 * @return The reference of the node
 */
Node& Dag::getNodeByPosition(const size_t position){
    return dag[position];
}

/**
 * @brief Dag::getRoot Get a reference to the root node
 * @return The reference to the root node
 */
Node& Dag::getRoot(){
    return dag[0];
}

/**
 * @brief Dag::addNode Add a node to the dag
 * @param newNode The node to add
 */
void Dag::addNode(const Node &newNode){
    dag.push_back(newNode);
}
