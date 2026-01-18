#ifndef OBJECTS_LIGHTS_GENERIC_LIGHT_H_
#define OBJECTS_LIGHTS_GENERIC_LIGHT_H_

#include "..\..\config.h"

struct GenericLight {
public:
    explicit GenericLight();
    virtual ~GenericLight();

    virtual void Print() = 0;

    LightType type;
};

#endif