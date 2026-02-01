#include <iostream>

#include "sphere.h"

Sphere::Sphere(int newId) : GenericObject(newId) {
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

void Sphere::BuildMiniBox() {
    this->lowX = this->cx - this->r;
    this->lowY = this->cy - this->r;
    this->lowZ = this->cz - this->r;
    this->highX = this->cx + this->r;
    this->highY = this->cy + this->r;
    this->highZ = this->cz + this->r;
}