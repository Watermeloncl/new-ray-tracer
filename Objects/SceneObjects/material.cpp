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