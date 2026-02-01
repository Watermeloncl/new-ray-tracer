#ifndef OBJECTS_SPECIAL_BOUNDING_BOX_H_
#define OBJECTS_SPECIAL_BOUNDING_BOX_H_

#include <vector>
#include <string>

#include "..\SceneObjects\genericObject.h"

struct BoundingBox {
public:
    BoundingBox();
    ~BoundingBox();
    void SplitCreate(std::vector<GenericObject*> objects);
    void Print(std::string prefix);

    bool leaf = false;
    BoundingBox* inner1 = nullptr;
    BoundingBox* inner2 = nullptr;

    GenericObject** objects = nullptr;
    int numObjects = 0;

    double lowX = 0.0;
    double lowY = 0.0;
    double lowZ = 0.0;

    double highX = 1.0;
    double highY = 1.0;
    double highZ = 1.0;

private:
    void ShrinkBox(std::vector<GenericObject*> objects);
    bool ObjectWithin(GenericObject* object);

};

#endif