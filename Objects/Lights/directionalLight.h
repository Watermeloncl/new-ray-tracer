#ifndef OBJECTS_LIGHTS_DIRECTIONAL_LIGHT_H_
#define OBJECTS_LIGHTS_DIRECTIONAL_LIGHT_H_

#include "genericLight.h"

struct DirectionalLight : public GenericLight {
public:
    DirectionalLight();
    ~DirectionalLight();

    double dx = 0;
    double dy = -1.0;
    double dz = 0;

    double r = 0.2;
    double g = 0.2;
    double b = 0.2;

    void Print();
};

#endif