#include <iostream>

#include "directionalLight.h"
#include "..\..\config.h"

DirectionalLight::DirectionalLight() {
    this->type = LightType::DIRECTIONAL;
}

DirectionalLight::~DirectionalLight() {
    
}

void DirectionalLight::Print() {
    std::cout << "Directional light: "
              << dx << " "
              << dy << " "
              << dz << " "
              << r << " "
              << g << " "
              << b << " "
              << std::endl;
}