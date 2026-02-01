#include "genericObject.h"

GenericObject::GenericObject(int newId) {
    this->id = newId;
}

GenericObject::~GenericObject() {
    delete this->material;
}