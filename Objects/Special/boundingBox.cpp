#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cfloat>
#include <string>

#include "boundingBox.h"
#include "..\SceneObjects\genericObject.h"
#include "..\SceneObjects\sphere.h"
#include "..\SceneObjects\polygon.h"
#include "..\..\config.h"

BoundingBox::BoundingBox() {

}

// does not delete objects, just array
BoundingBox::~BoundingBox() {
    delete this->inner1;
    delete this->inner2;

    delete[] objects;
}

void BoundingBox::SplitCreate(std::vector<GenericObject*> objects) {
    this->ShrinkBox(objects);

    if(objects.size() <= MAX_OBJECTS_IN_BOUNDING_BOX) {
        this->leaf = true;
        this->objects = new GenericObject*[MAX_OBJECTS_IN_BOUNDING_BOX];

        for(GenericObject* o : objects) {
            this->objects[this->numObjects] = o;
            (this->numObjects)++;
        }
        return;
    }

    double deltaX = std::abs(this->highX - this->lowX);
    double deltaY = std::abs(this->highY - this->lowY);
    double deltaZ = std::abs(this->highZ - this->lowZ);
    double maxDelta = std::max({deltaX, deltaY, deltaZ});

    BoundingBox* inner1 = new BoundingBox();
    BoundingBox* inner2 = new BoundingBox();

    inner1->lowX = this->lowX;
    inner1->lowY = this->lowY;
    inner1->lowZ = this->lowZ;
    inner1->highX = this->highX;
    inner1->highY = this->highY;
    inner1->highZ = this->highZ;
    inner2->lowX = this->lowX;
    inner2->lowY = this->lowY;
    inner2->lowZ = this->lowZ;
    inner2->highX = this->highX;
    inner2->highY = this->highY;
    inner2->highZ = this->highZ;

    if(maxDelta == deltaX) {
        inner1->highX = inner1->highX - (deltaX / 2.0);
        inner2->lowX = inner1->highX;
    } else if(maxDelta == deltaY) {
        inner1->highY = inner1->highY - (deltaY / 2.0);
        inner2->lowY = inner1->highY;
    } else {
        inner1->highZ = inner1->highZ - (deltaZ / 2.0);
        inner2->lowZ = inner1->highZ;
    }

    this->inner1 = inner1;
    this->inner2 = inner2;

    std::vector<GenericObject*> inner1Objects;
    std::vector<GenericObject*> inner2Objects;

    inner1Objects.reserve(MAX_OBJECTS_IN_BOUNDING_BOX);
    inner2Objects.reserve(MAX_OBJECTS_IN_BOUNDING_BOX);

    for(GenericObject* o : objects) {
        if(inner1->ObjectWithin(o)) {
            inner1Objects.push_back(o);
        }

        if(inner2->ObjectWithin(o)) {
            inner2Objects.push_back(o);
        }
    }

    this->inner1->SplitCreate(inner1Objects);
    this->inner2->SplitCreate(inner2Objects);
}

void BoundingBox::Print(std::string prefix) {
    std::cout << this->lowX << "," << this->lowY << "," << this->lowZ << "; " << this->highX << "," << this->highY << "," << this->highZ << std::endl;

    if(this->leaf) {
        for(int i = 0; i < this->numObjects; i++) {
            if(this->objects[i]->type == ObjectType::SPHERE) {
                std::cout << prefix << "Sphere" << std::endl;
            } else if(this->objects[i]->type == ObjectType::POLYGON) {
                std::cout << prefix << "Polygon" << std::endl;
            }
        }
    } else {
        this->inner1->Print(prefix + "-");
        this->inner2->Print(prefix + "-");
    }
}

void BoundingBox::ShrinkBox(std::vector<GenericObject*> objects) {
    double lowX = DBL_MAX;
    double lowY = DBL_MAX;
    double lowZ = DBL_MAX;
    double highX = -DBL_MAX;
    double highY = -DBL_MAX;
    double highZ = -DBL_MAX;

    for(int i = 0; i < ((int)objects.size()); i++) {
        lowX = std::min(lowX, objects[i]->lowX);
        lowY = std::min(lowY, objects[i]->lowY);
        lowZ = std::min(lowZ, objects[i]->lowZ);
        highX = std::max(highX, objects[i]->highX);
        highY = std::max(highY, objects[i]->highY);
        highZ = std::max(highZ, objects[i]->highZ);
    }

    if(this->lowX < lowX) {this->lowX = lowX;}
    if(this->lowY < lowY) {this->lowY = lowY;}
    if(this->lowZ < lowZ) {this->lowZ = lowZ;}
    if(this->highX > highX) {this->highX = highX;}
    if(this->highY > highY) {this->highY = highY;}
    if(this->highZ > highZ) {this->highZ = highZ;}
}

bool BoundingBox::ObjectWithin(GenericObject* object) {
    if((object->lowX <= this->highX) &&
       (object->highX >= this->lowX) &&
       (object->lowY <= this->highY) &&
       (object->highY >= this->lowY) &&
       (object->lowZ <= this->highZ) &&
       (object->highZ >= this->lowZ)) {

        return true;
    }

    return false;
}