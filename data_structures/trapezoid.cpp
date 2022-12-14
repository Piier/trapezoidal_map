#include "trapezoid.h"
#define BB_LEFTP 2
#define BB_RIGHTP 1
#define BB_TOP 0
#define BB_BOTTOM 1

/**
 * @brief Trapezoid::Trapezoid Constructor that create the bb
 */
Trapezoid::Trapezoid():
leftp(BB_LEFTP), rightp(BB_RIGHTP), top(BB_TOP), bottom(BB_BOTTOM),
adjBottomLeft(SIZE_MAX),adjBottomRight(SIZE_MAX),adjTopLeft(SIZE_MAX),adjTopRight(SIZE_MAX),
deleted(false),nodeId(0)
{}

/**
 * @brief Trapezoid Constructor with the perimeter
 * @param l Left point
 * @param r Right point
 * @param t Top segment
 * @param b Bottom segment
 */
Trapezoid::Trapezoid(const size_t l, const size_t r, const size_t t, const size_t b):
leftp(l), rightp(r), top(t), bottom(b),
adjBottomLeft(SIZE_MAX),adjBottomRight(SIZE_MAX),adjTopLeft(SIZE_MAX),adjTopRight(SIZE_MAX),
deleted(false),nodeId(0)
{}

//Getters
size_t Trapezoid::getLeft() const{
    return leftp;
}

size_t Trapezoid::getRight() const{
    return rightp;
}

size_t Trapezoid::getTop() const{
    return top;
}

size_t Trapezoid::getBottom() const{
    return bottom;
}

size_t Trapezoid::getAdjBottomLeft() const{
    return adjBottomLeft;
}

size_t Trapezoid::getAdjBottomRight() const{
    return adjBottomRight;
}

size_t Trapezoid::getAdjTopLeft() const{
    return adjTopLeft;
}

size_t Trapezoid::getAdjTopRight() const{
    return adjTopRight;
}

bool Trapezoid::isDeleted() const{
    return deleted;
}

size_t Trapezoid::getNodeId() const{
    return nodeId;
}

//Setters
void Trapezoid::setLeft(const size_t newLeft){
    leftp=newLeft;
}

void Trapezoid::setRight(const size_t newRight){
    rightp=newRight;
}

void Trapezoid::setTop(const size_t newTop){
    top=newTop;
}

void Trapezoid::setBottom(const size_t newBottom){
    bottom=newBottom;
}

void Trapezoid::setAdjBottomLeft(const size_t newAdj){
    adjBottomLeft=newAdj;
}

void Trapezoid::setAdjBottomRight(const size_t newAdj){
    adjBottomRight=newAdj;
}

void Trapezoid::setAdjTopLeft(const size_t newAdj){
    adjTopLeft=newAdj;
}

void Trapezoid::setAdjTopRight(const size_t newAdj){
    adjTopRight=newAdj;
}


void Trapezoid::setDeleted(){
    deleted=true;
}

void Trapezoid::setNodeId(const size_t newId){
    nodeId=newId;
}








