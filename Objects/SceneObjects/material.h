#ifndef OBJECTS_SCENE_OBJECTS_MATERIAL_H_
#define OBJECTS_SCENE_OBJECTS_MATERIAL_H_

struct Material {
public:
    Material();
    ~Material();

    // Do we need K or od or os?
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
    double nit = 1.0;
    
    // speed up values
    double ar = 0;
    double ag = 0;
    double ab = 0;

    double kdodr = 0;
    double kdodg = 0;
    double kdodb = 0;
    
    double ksosr = 0;
    double ksosg = 0;
    double ksosb = 0;

    void Print();
};

#endif