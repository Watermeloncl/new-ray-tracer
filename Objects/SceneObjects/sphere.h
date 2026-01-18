#ifndef OBJECTS_SCENE_OBJECTS_SPHERE_H_
#define OBJECTS_SCENE_OBJECTS_SPHERE_H_

#include "genericObject.h"

struct Sphere : public GenericObject {
public:
    Sphere();
    ~Sphere();

    float cx = 0;
    float cy = 0;
    float cz = 0;

    float r = 1.0;

    void Print();
};

#endif