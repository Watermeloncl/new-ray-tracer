#include <iostream>

#include "areaLight.h"
#include "..\SceneObjects\polygon.h"

AreaLight::AreaLight(int newId) : Polygon(newId) {
    this->GenericLight::type = LightType::AREA;
    this->isLight = true;
}

AreaLight::~AreaLight() {

}

void AreaLight::Print() {
    std::cout << "Area Light: ("
              << numPoints << "): " << std::endl;
    for(int i = 0; i < numPoints; i++) {
        std::cout << i << ": "
                  << points[i*3] << " "
                  << points[i*3 + 1] << " "
                  << points[i*3 + 2] << " "
                  << std::endl;
    }
    std::cout << "Normal: " << nx << " " << ny << " " << nz << std::endl;
    std::cout << "Color: "
              << r << " "
              << g << " "
              << b << std::endl;
}
