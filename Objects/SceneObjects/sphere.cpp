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

void Sphere::CalcNormal(double cpx, double cpy, double cpz, double& nx, double& ny, double& nz) {
    double invr = 1 / this->r;
    nx = (cpx - this->cx) * invr;
    ny = (cpy - this->cy) * invr;
    nz = (cpz - this->cz) * invr;
}