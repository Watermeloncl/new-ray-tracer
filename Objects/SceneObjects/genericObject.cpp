#include "genericObject.h"

GenericObject::GenericObject() {

}

GenericObject::~GenericObject() {
    delete this->material;
}