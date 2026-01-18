#ifndef OBJECTS_SCENE_OBJECTS_GENERIC_OBJECT_H_
#define OBJECTS_SCENE_OBJECTS_GENERIC_OBJECT_H_

#include "material.h"

struct GenericObject {
public:
    explicit GenericObject();
    virtual ~GenericObject();
    virtual void Print() = 0;

    Material* material = nullptr;
};

#endif