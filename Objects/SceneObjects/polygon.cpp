#include <iostream>
#include <algorithm>

#include "polygon.h"
#include "..\..\config.h"

Polygon::Polygon(int newId) : GenericObject(newId) {
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

void Polygon::BuildMiniBox() {
    this->lowX = this->points[0];
    this->lowY = this->points[1];
    this->lowZ = this->points[2];

    this->highX = this->points[0];
    this->highY = this->points[1];
    this->highZ = this->points[2];

    for(int i = 1; i < this->numPoints; i++) {
        this->lowX = std::min(this->lowX, this->points[i*3]);
        this->lowY = std::min(this->lowY, this->points[i*3 + 1]);
        this->lowZ = std::min(this->lowZ, this->points[i*3 + 2]);

        this->highX = std::max(this->highX, this->points[i*3]);
        this->highY = std::max(this->highY, this->points[i*3 + 1]);
        this->highZ = std::max(this->highZ, this->points[i*3 + 2]);
    }
}
