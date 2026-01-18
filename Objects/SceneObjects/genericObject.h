#ifndef OBJECTS_SCENE_OBJECTS_GENERIC_OBJECT_H_
#define OBJECTS_SCENE_OBJECTS_GENERIC_OBJECT_H_

#include "material.h"
#include "..\..\config.h"

struct GenericObject {
public:
    explicit GenericObject();
    virtual ~GenericObject();
    virtual void Print() = 0;

    ObjectType type;
    Material* material = nullptr;
};

#endif