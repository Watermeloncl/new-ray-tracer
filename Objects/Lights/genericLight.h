#ifndef OBJECTS_LIGHTS_GENERIC_LIGHT_H_
#define OBJECTS_LIGHTS_GENERIC_LIGHT_H_

struct GenericLight {
public:
    explicit GenericLight();
    virtual ~GenericLight();

    virtual void Print() = 0;
};

#endif