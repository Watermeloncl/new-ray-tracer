#include <iostream>

#include "sceneInfo.h"
#include "..\Lights\genericLight.h"
#include "..\SceneObjects\genericObject.h"
#include "..\..\config.h"

SceneInfo::SceneInfo() {
    this->lights = new GenericLight*[MAX_LIGHTS];
    for(int i = 0; i < MAX_LIGHTS; i++) {
        this->lights[i] = nullptr;
    }

    this->sceneObjects = new GenericObject*[MAX_OBJECTS];
    for(int i = 0; i < MAX_OBJECTS; i++) {
        this->sceneObjects[i] = nullptr;
    }
}

SceneInfo::~SceneInfo() {
    for(int i = 0; i < numLights; i++) {
        delete this->lights[i];
    }
    delete[] this->lights;

    for(int i = 0; i < numSceneObjects; i++) {
        delete this->sceneObjects[i];
    }
    delete[] this->sceneObjects;
    delete this->startingStack;
}

void SceneInfo::Print() {
    std::cout << "Scene info" << std::endl;
    std::cout << "view: " << worldSpace << " "
              << viewDistance << std::endl;
    std::cout << "amb/back: "
              << ambR << " "
              << ambG << " "
              << ambB << " "
              << bckR << " "
              << bckG << " "
              << bckB << " "
              << std::endl;

    std::cout << "Num lights: " << numLights << std::endl;
    for(int i = 0; i < numLights; i++) {
        lights[i]->Print();
    }

    std::cout << "Num objects: " << numSceneObjects << std::endl;
    for(int i = 0; i < numSceneObjects; i++) {
        sceneObjects[i]->Print();
    }

    std::cout << "End scene" << std::endl;

}