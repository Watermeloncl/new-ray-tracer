#ifndef OBJECTS_SPECIAL_SCENE_INFO_H_
#define OBJECTS_SPECIAL_SCENE_INFO_H_

#include <stack>

#include "..\Lights\genericLight.h"
#include "..\SceneObjects\genericObject.h"
#include "boundingBox.h"

struct SceneInfo {
public:
    SceneInfo();
    ~SceneInfo();

    GenericLight** lights = nullptr;
    GenericObject** sceneObjects = nullptr;
    std::stack<double>* startingStack = nullptr;

    BoundingBox* headBox = nullptr;

    int numLights = 0;
    int numSceneObjects = 0;

    double worldSpace = 0.1;
    double viewDistance = 5.0;
    
    double ambR = 0.1;
    double ambG = 0.1;
    double ambB = 0.1;

    double bckR = 0.0;
    double bckG = 0.0;
    double bckB = 0.0;

    void Print();
    void PrintTree();
};

#endif