#ifndef NODE_H
#define NODE_H
#include <stddef.h>
#include <stddef.h>
#include <vector>
#include <cg3/geometry/point2.h>
#include <cg3/geometry/polygon2.h>
enum Type{
    T_Point,
    T_Segment,
    T_Trapezoid
};

class Node{

private:
    Type type;
    size_t leftChild, rightChild;
    size_t element;
    size_t id;

public:
    Node(const size_t e, Type t);
    Node(const size_t e, Type t, size_t lc, size_t rc);

    Type getType() const;
    size_t getLeftChild() const;
    size_t getRightChild() const;
    size_t getElement() const;
    size_t getId() const;

    void setElement(const size_t newElement);
    void setType(Type newType);
    void setLeftChild(const size_t newLeftChild);
    void setRightChild(const size_t newRightChild);
    void setId(const size_t newId);
};

#endif // NODE_H
