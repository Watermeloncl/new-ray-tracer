#ifndef OBJECTS_SCENE_OBJECTS_POLYGON_H_
#define OBJECTS_SCENE_OBJECTS_POLYGON_H_

#include "genericObject.h"

struct Polygon : public GenericObject {
public:
    Polygon(int newId);
    ~Polygon();
    
    void Print();
    void CalcNormal(double cpx, double cpy, double cpz, double& nx, double& ny, double& nz);
    void BuildMiniBox();

    int numPoints = 0;
    double* points = nullptr;

    //speed up values
    double nx = 0.0;
    double ny = 0.0;
    double nz = 0.0;
};

#endif