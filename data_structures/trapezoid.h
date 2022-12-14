#ifndef TRAPEZOID_H
#define TRAPEZOID_H
#include "node.h"

class Trapezoid{

private:
    size_t leftp, rightp;
    size_t top, bottom;
    size_t adjBottomLeft, adjBottomRight, adjTopLeft, adjTopRight;
    bool deleted;
    size_t nodeId;

public:
    Trapezoid();
    Trapezoid(const size_t l, const size_t r, const size_t t, const size_t b);

    size_t getLeft() const;
    size_t getRight() const;
    size_t getTop() const;
    size_t getBottom() const;
    size_t getAdjTopRight() const;
    size_t getAdjTopLeft() const;
    size_t getAdjBottomRight() const;
    size_t getAdjBottomLeft() const;
    size_t getNodeId() const;
    bool isDeleted() const;

    void setLeft(const size_t newLeft);
    void setRight(const size_t newRight);
    void setBottom(const size_t newBottom);
    void setTop(const size_t newTop);
    void setAdjTopRight(const size_t newAdj);
    void setAdjTopLeft(const size_t newAdj);
    void setAdjBottomRight(const size_t newAdj);
    void setAdjBottomLeft(const size_t newAdj);
    void setDeleted();
    void setNodeId(const size_t newId);

};


#endif // TRAPEZOID_H
