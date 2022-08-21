#ifndef TRAPEZOID_H
#define TRAPEZOID_H
#include "node.h"

class Trapezoid{

private:
    size_t leftp, rightp;
    size_t top, bottom;
    size_t adjTopRight, adjTopLeft, adjBottomRight, adjBottomLeft;
    bool deleted;

public:
    Trapezoid();

    size_t getLeft() const;
    size_t getRight() const;
    size_t getTop() const;
    size_t getBottom() const;
    size_t getAdjTopRight() const;
    size_t getAdjTopLeft() const;
    size_t getAdjBottomRight() const;
    size_t getAdjBottomLeft() const;
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

};


#endif // TRAPEZOID_H
