#include "node.h"

Node::Node(){
    element=SIZE_MAX;
    type=Type::T_Point;
    leftChild=SIZE_MAX;
    rightChild=SIZE_MAX;
    id=0;
}

/**
 * @brief Node::Node Constuctor
 * @param e Position of the element in the trapezoidal map
 * @param t Type of the element in the trapezoidal map
 */
Node::Node(const size_t e, Type t){
    element=e;
    type=t;
    leftChild=SIZE_MAX;
    rightChild=SIZE_MAX;
    id=0;
}

/**
 * @brief Node::Node Constructor
 * @param e Position of the element in the trapezoidal map
 * @param t Type of the element in the trapezoidal map
 * @param lc Left child in the dag
 * @param rc Right child in the dag
 */
Node::Node(const size_t e, Type t, size_t lc, size_t rc){
    element=e;
    type=t;
    leftChild=lc;
    rightChild=rc;
    id=0;
}

//Getters
Type Node::getType() const{
    return type;
}

size_t Node::getLeftChild() const{
    return leftChild;
}

size_t Node::getRightChild() const{
    return rightChild;
}

size_t Node::getElement() const{
    return element;
}

size_t Node::getId() const{
    return id;
}

//Setters
void Node::setElement(const size_t newElement){
    element=newElement;
}

void Node::setType(Type newType){
    type=newType;
}

void Node::setLeftChild(const size_t newLeftChild){
    leftChild=newLeftChild;
}

void Node::setRightChild(const size_t newRightChild){
    rightChild=newRightChild;
}

void Node::setId(const size_t newId){
    id=newId;
}

void Node::setElementAndType(const size_t e, Type t){
    element=e;
    type=t;
}

void Node::setChildren(const size_t lc, const size_t rc){
    leftChild=lc;
    rightChild=rc;
}
