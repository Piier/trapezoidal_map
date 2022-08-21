#ifndef NODE_H
#define NODE_H
#include <stddef.h>
#include <stddef.h>
#include <vector>
#include <cg3/geometry/point2.h>
#include <cg3/geometry/polygon2.h>
enum Type{
    Vertex,
    Edge,
    Face
};

class Node{

private:
    Type type;
    size_t leftChild, rightChild;
    size_t element;

public:
    Node(const size_t e, Type t);

    Type getType() const;
    size_t getLeftChild() const;
    size_t getRightChild() const;
    size_t getElement() const;

    void setElement(const size_t newElement);
    void setType(Type newType);
    void setLeftChild(const size_t newLeftChild);
    void setRightChild(const size_t newRightChild);
};

#endif // NODE_H
