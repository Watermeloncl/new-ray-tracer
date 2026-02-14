#ifndef OBJECTS_LIGHTS_AREA_LIGHT_H_
#define OBJECTS_LIGHTS_AREA_LIGHT_H_

#include "genericLight.h"
#include "..\SceneObjects\polygon.h"

struct AreaLight : public GenericLight, public Polygon {
public:
    AreaLight(int newId);
    ~AreaLight();

    void Print();
};

#endif