#ifndef OBJECTS_LIGHTS_GENERIC_LIGHT_H_
#define OBJECTS_LIGHTS_GENERIC_LIGHT_H_

#include "..\..\config.h"

struct GenericLight {
public:
    explicit GenericLight();
    virtual ~GenericLight();

    virtual void Print() = 0;
    
    double r = 0.2;
    double g = 0.2;
    double b = 0.2;

    LightType type;
};

#endif