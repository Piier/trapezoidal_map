#ifndef DRAWABLE_TRAPEZOIDMAP_H
#define DRAWABLE_TRAPEZOIDMAP_H
#include "data_structures/trapezoidalmap.h"
#include <cg3/viewer/interfaces/drawable_object.h>
class DrawableTrapezoidalMap : public TrapezoidalMap, public cg3::DrawableObject{

private:
    std::vector<cg3::Color> colors;
    size_t query;
    const cg3::Color black = cg3::Color(0,0,0);

public:
     DrawableTrapezoidalMap();

     void draw() const;
     cg3::Point3d sceneCenter() const;
     double sceneRadius() const;

     cg3::Color randColor();
     void addColors(const size_t n);
     void setQuery(const size_t val);

};


#endif // DRAWABLE_TRAPEZOIDMAP_H
