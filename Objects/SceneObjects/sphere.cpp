#include <iostream>

#include "sphere.h"

Sphere::Sphere() {
    this->type = ObjectType::SPHERE;
}

Sphere::~Sphere() {
    
}

void Sphere::Print() {
    std::cout << "Sphere: "
              << cx << " "
              << cy << " "
              << cz << " "
              << r << " "
              << std::endl;
    this->material->Print();
}