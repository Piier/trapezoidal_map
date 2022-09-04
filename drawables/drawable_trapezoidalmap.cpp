#include "drawable_trapezoidalmap.h"
#include "utils/algorithms_utils.h"
#include <cg3/viewer/opengl_objects/opengl_objects2.h>

#define COLOR_UPPER 255
#define COLOR_LOWER 30

DrawableTrapezoidalMap::DrawableTrapezoidalMap(): query(SIZE_MAX){
    TrapezoidalMap();
    colors.push_back(randColor());//The first color
}

void DrawableTrapezoidalMap::draw() const{

    size_t i = 0;
    for(const Trapezoid& trapezoid : trapezoidsTable){      
        if(!trapezoid.isDeleted()){

            std::array<cg3::Point2d, 4> vertices = {cg3::Point2d(pointsTable[trapezoid.getLeft()].x(), AlgorithmsUtils::getYGivenX(segmentsTable[trapezoid.getTop()], pointsTable[trapezoid.getLeft()].x())),
                                                    cg3::Point2d(pointsTable[trapezoid.getRight()].x(), AlgorithmsUtils::getYGivenX(segmentsTable[trapezoid.getTop()], pointsTable[trapezoid.getRight()].x())),
                                                    cg3::Point2d(pointsTable[trapezoid.getRight()].x(), AlgorithmsUtils::getYGivenX(segmentsTable[trapezoid.getBottom()], pointsTable[trapezoid.getRight()].x())),
                                                    cg3::Point2d(pointsTable[trapezoid.getLeft()].x(), AlgorithmsUtils::getYGivenX(segmentsTable[trapezoid.getBottom()], pointsTable[trapezoid.getLeft()].x()))};

            cg3::opengl::drawLine2(vertices[0],vertices[3], black, 1);
            cg3::opengl::drawLine2(vertices[1],vertices[2], black, 1);
            cg3::opengl::drawQuad2(vertices, i==query ? black : colors[i], 1, true);
        }
        i++;
    }
}

/**
 * @brief DrawableTrapezoidalMap::randColor Generate a random color
 * @return A colot
 */
cg3::Color DrawableTrapezoidalMap::randColor(){
    int i = COLOR_UPPER - COLOR_LOWER +1;
    return cg3::Color(((AlgorithmsUtils::xorshf96()-COLOR_LOWER)%i+i)%i+COLOR_LOWER,
                      ((AlgorithmsUtils::xorshf96()-COLOR_LOWER)%i+i)%i+COLOR_LOWER,
                      ((AlgorithmsUtils::xorshf96()-COLOR_LOWER)%i+i)%i+COLOR_LOWER);
}

/**
 * @brief DrawableTrapezoidalMap::addColors Add colors in the vector
 * @param n Size to reach
 */
void DrawableTrapezoidalMap::addColors(size_t n){
    size_t i = colors.size();
    while(i<n){
        colors.push_back(randColor());
        i++;
    }
}

void DrawableTrapezoidalMap::setQuery(size_t val){
    query=val;
}

cg3::Point3d DrawableTrapezoidalMap::sceneCenter() const
{
    const cg3::BoundingBox2& boundingBox = cg3::BoundingBox2();
    return cg3::Point3d(boundingBox.center().x(), boundingBox.center().y(), 0);
}

double DrawableTrapezoidalMap::sceneRadius() const
{
    const cg3::BoundingBox2& boundingBox = cg3::BoundingBox2();
    return boundingBox.diag();
}

