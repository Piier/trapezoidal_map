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
    size_t element;
    Type type;
    size_t leftChild, rightChild;

public:
    Node();
    Node(const size_t e, const Type t);
    Node(const size_t e, const Type t, const size_t lc, const size_t rc);

    Type getType() const;
    size_t getLeftChild() const;
    size_t getRightChild() const;
    size_t getElement() const;

    void setElement(const size_t newElement);
    void setType(const Type newType);
    void setLeftChild(const size_t newLeftChild);
    void setRightChild(const size_t newRightChild);
    void setId(const size_t newId);
    void setElementAndType(const size_t e, Type t);
    void setChildren(const size_t lc, const size_t rc);
};

#endif // NODE_H
