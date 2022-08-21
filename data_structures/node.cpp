#include "node.h"

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
