#ifndef OBJECTS_LIGHTS_POINT_LIGHT_H_
#define OBJECTS_LIGHTS_POINT_LIGHT_H_

#include "genericLight.h"

struct PointLight : public GenericLight {
public:
    PointLight();
    ~PointLight();
    void Print();

    double cx = 0.0;
    double cy = 0.0;
    double cz = 0.0;
};

#endif