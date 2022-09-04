#ifndef DRAWABLE_TRAPEZOIDMAP_H
#define DRAWABLE_TRAPEZOIDMAP_H
#include "data_structures/trapezoidalmap.h"
#include <cg3/viewer/interfaces/drawable_object.h>
class DrawableTrapezoidalMap : public TrapezoidalMap, public cg3::DrawableObject{

private:
    std::vector<cg3::Color> colors;
    size_t query;


public:
     DrawableTrapezoidalMap();

     void draw() const;
     cg3::Point3d sceneCenter() const;
     double sceneRadius() const;

     cg3::Color randColor();
     void addColors(size_t n);
     void setQuery(size_t val);

};


#endif // DRAWABLE_TRAPEZOIDMAP_H
