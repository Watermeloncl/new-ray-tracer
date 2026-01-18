#ifndef OBJECTS_SCENE_OBJECTS_SPHERE_H_
#define OBJECTS_SCENE_OBJECTS_SPHERE_H_

#include "genericObject.h"

struct Sphere : public GenericObject {
public:
    Sphere();
    ~Sphere();

    double cx = 0;
    double cy = 0;
    double cz = 0;

    double r = 1.0;

    void Print();
};

#endif