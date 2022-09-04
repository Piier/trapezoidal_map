#ifndef DRAWABLE_TRAPEZOIDMAP_H
#define DRAWABLE_TRAPEZOIDMAP_H
#include "data_structures/trapezoidalmap.h"
#include <cg3/viewer/interfaces/drawable_object.h>
class DrawableTrapezoidalMap : public TrapezoidalMap, public cg3::DrawableObject{

private:
    std::vector<cg3::Color> colors;


public:
     DrawableTrapezoidalMap();

     void draw() const;
     cg3::Point3d sceneCenter() const;
     double sceneRadius() const;

     cg3::Color randColor();
     void addColors(size_t n);

};


#endif // DRAWABLE_TRAPEZOIDMAP_H
