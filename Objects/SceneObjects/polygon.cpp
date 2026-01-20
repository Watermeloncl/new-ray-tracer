#include <iostream>

#include "polygon.h"
#include "..\..\config.h"

Polygon::Polygon() {
    this->type = ObjectType::POLYGON;
}

Polygon::~Polygon() {

}

void Polygon::Print() {
    std::cout << "Polygon ("
              << numPoints << "): " << std::endl;
    for(int i = 0; i < numPoints; i++) {
        std::cout << i << ": "
                  << points[i*3] << " "
                  << points[i*3 + 1] << " "
                  << points[i*3 + 2] << " "
                  << std::endl;
    }
    std::cout << "Normal: " << nx << " " << ny << " " << nz << std::endl;
    this->material->Print();
}

void Polygon::CalcNormal(double cpx, double cpy, double cpz, double& nx, double& ny, double& nz) {
    nx = this->nx;
    ny = this->ny;
    nz = this->nz;
}
