#include <iostream>

#include "material.h"

Material::Material() {

}

Material::~Material() {
    
}

void Material::Print() {
    std::cout << "Material: "
              << kd << " "
              << ks << " "
              << ka << " "
              << odr << " "
              << odg << " "
              << odb << " "
              << osr << " "
              << osg << " "
              << osb << " "
              << kgls << " "
              << refl << " "
              << trans << " "
              << nit << " "
              << std::endl;
}

Material* Material::Copy() {
    Material* mat = new Material();
    mat->ar = this->ar;
    mat->ag = this->ag;
    mat->ab = this->ab;
    mat->kdodr = this->kdodr;
    mat->kdodg = this->kdodg;
    mat->kdodb = this->kdodb;
    mat->ksosr = this->ksosr;
    mat->ksosg = this->ksosg;
    mat->ksosb = this->ksosb;
    return mat;
}