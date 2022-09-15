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
Node &Dag::getNodeByPosition(const size_t position){
    return dag[position];
}

/**
 * @brief Dag::getRoot Get a reference to the root node
 * @return The reference to the root node
 */
Node &Dag::getRoot(){
    return dag[0];
}

/**
 * @brief Dag::getSize Get the size of the dag
 * @return The size of the dag
 */
size_t Dag::getSize() const{
    return dag.size();
}

/**
 * @brief Dag::addNode Add a node to the dag
 * @param newNode The node to add
 */
size_t Dag::addNode(const Node &newNode){
    dag.push_back(newNode);
    return dag.size()-1;
}

void Dag::clear(){
    dag.clear();
    //Re-initialize
    dag.push_back(Node(0,Type::T_Trapezoid));
}
