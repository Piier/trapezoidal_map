#include "trapezoid.h"

/**
 * @brief Trapezoid::Trapezoid Constructor
 */
Trapezoid::Trapezoid(){
    leftp=SIZE_MAX;
    rightp=SIZE_MAX;
    top=SIZE_MAX;;
    bottom=SIZE_MAX;
    adjBottomLeft=SIZE_MAX;
    adjBottomRight=SIZE_MAX;
    adjTopLeft=SIZE_MAX;
    adjTopRight=SIZE_MAX;
    deleted=false;
}


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









