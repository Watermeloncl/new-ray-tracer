#ifndef OBJECTS_SCENE_OBJECTS_SPHERE_H_
#define OBJECTS_SCENE_OBJECTS_SPHERE_H_

#include "genericObject.h"

struct Sphere : public GenericObject {
public:
    Sphere(int newId);
    ~Sphere();
    
    void Print();
    void CalcNormal(double cpx, double cpy, double cpz, double& nx, double& ny, double& nz);
    void BuildMiniBox();

    double cx = 0;
    double cy = 0;
    double cz = 0;

    double r = 1.0;
    
    // speed up values
    double cxcx = 0;
    double cycy = 0;
    double czcz = 0;
    double rr = 1;


};

#endif