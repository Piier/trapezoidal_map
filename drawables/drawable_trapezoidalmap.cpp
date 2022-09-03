#include "drawable_trapezoidalmap.h"
#include "utils/algorithms_utils.h"
#include <cg3/viewer/opengl_objects/opengl_objects2.h>


DrawableTrapezoidalMap::DrawableTrapezoidalMap(){
    TrapezoidalMap();
}

void DrawableTrapezoidalMap::draw() const{

    for(const Trapezoid& trapezoid : trapezoidsTable){      
        if(!trapezoid.isDeleted()){



            std::array<cg3::Point2d, 4> vertices = {cg3::Point2d(pointsTable[trapezoid.getLeft()].x(), AlgorithmsUtils::getYGivenX(segmentsTable[trapezoid.getTop()], pointsTable[trapezoid.getLeft()].x())),
                                                    cg3::Point2d(pointsTable[trapezoid.getRight()].x(), AlgorithmsUtils::getYGivenX(segmentsTable[trapezoid.getTop()], pointsTable[trapezoid.getRight()].x())),
                                                    cg3::Point2d(pointsTable[trapezoid.getRight()].x(), AlgorithmsUtils::getYGivenX(segmentsTable[trapezoid.getBottom()], pointsTable[trapezoid.getRight()].x())),
                                                    cg3::Point2d(pointsTable[trapezoid.getLeft()].x(), AlgorithmsUtils::getYGivenX(segmentsTable[trapezoid.getBottom()], pointsTable[trapezoid.getLeft()].x()))};
            cg3::opengl::drawQuad2(vertices, QColor(), 1, false);
        }
    }
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
