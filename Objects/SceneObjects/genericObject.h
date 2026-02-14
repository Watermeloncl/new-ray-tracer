#ifndef OBJECTS_SCENE_OBJECTS_GENERIC_OBJECT_H_
#define OBJECTS_SCENE_OBJECTS_GENERIC_OBJECT_H_

#include "material.h"
#include "..\..\config.h"

struct GenericObject {
public:
    explicit GenericObject(int newId);
    virtual ~GenericObject();
    virtual void Print() = 0;
    virtual void CalcNormal(double cpx, double cpy, double cpz, double& nx, double& ny, double& nz) = 0;
    virtual void BuildMiniBox() = 0;

    ObjectType type;
    Material* material = nullptr;
    int id = 0.0;

    double lowX = 0.0;
    double lowY = 0.0;
    double lowZ = 0.0;
    double highX = 0.0;
    double highY = 0.0;
    double highZ = 0.0;
};

#endif