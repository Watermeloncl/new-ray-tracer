#ifndef OBJECTS_SCENE_OBJECTS_MATERIAL_H_
#define OBJECTS_SCENE_OBJECTS_MATERIAL_H_

struct Material {
public:
    Material();
    ~Material();

    double kd = 0;
    double ks = 0;
    double ka = 0;

    double odr = 0;
    double odg = 0;
    double odb = 0;
    
    double osr = 0;
    double osg = 0;
    double osb = 0;

    double kgls = 4.0;
    double refl = 0.0;
    double trans = 0.0;

    void Print();
};

#endif