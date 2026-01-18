#include <iostream>

#include "directionalLight.h"

DirectionalLight::DirectionalLight() {

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