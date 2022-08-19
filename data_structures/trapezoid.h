#ifndef TRAPEZOID_H
#define TRAPEZOID_H

class Trapezoid{

private:
    int leftp, rightp;
    int top, bottom;
    int adjTopRight, adjTopLeft, adjBottomRight, adjBottomLeft;


public:
    Trapezoid();

    int getLeft();
    int getRight();
    int getTop();
    int getBottom();
    int getAdjTopRight();
    int getAdjTopLeft();
    int getAdjBottomRight();
    int getAdjBottomLeft();

    void setLeft(const int left);
    void setRight(const int right);
    void setBottom(const int bottom);
    void setTop(const int top);
    void setAdjTopRight(const int newAdj);
    void setAdjTopLeft(const int newAdj);
    void setAdjBottomRight(const int newAdj);
    void setAdjBottomLeft(const int newAdj);

};


#endif // TRAPEZOID_H
