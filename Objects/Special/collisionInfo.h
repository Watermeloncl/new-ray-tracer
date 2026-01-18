#ifndef OBJECTS_SPECIAL_COLLISION_INFO_H_
#define OBJECTS_SPECIAL_COLLISION_INFO_H_

struct CollisionInfo {
public:
    CollisionInfo();
    ~CollisionInfo();

    double t = 0;
    int oIndex = 0;

    double cpx = 0;
    double cpy = 0;
    double cpz = 0;
};

#endif