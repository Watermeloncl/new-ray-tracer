#include <iostream>

#include "pointLight.h"
#include "..\..\config.h"

PointLight::PointLight() {
    this->type = LightType::POINT;
}

PointLight::~PointLight() {

}

void PointLight::Print() {
    std::cout << "Point Light: "
              << cx << " "
              << cy << " "
              << cz << std::endl;
}