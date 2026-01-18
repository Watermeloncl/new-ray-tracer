#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "parser.h"
#include "..\Objects\Special\sceneInfo.h"
#include "..\Objects\Lights\directionalLight.h"
#include "..\Objects\SceneObjects\sphere.h"
#include "..\Objects\SceneObjects\material.h"
#include "..\config.h"

Parser::Parser() {

}

Parser::~Parser() {

}

void Parser::ParseInput(SceneInfo* sceneInfo) {
    std::ifstream file(IN_FILE_NAME);
    if (!file.is_open()) {
        std::cout << "Could not open file!" << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if(cmd == "WorldSpace") {
            ss >> sceneInfo->worldSpace;
        } else if(cmd == "ViewDistance") {
            ss >> sceneInfo->viewDistance;
        } else if(cmd == "AmbientLight") {
            ss >> sceneInfo->ambR >> sceneInfo->ambG >> sceneInfo->ambB;
        } else if(cmd == "BackgroundColor") {
            ss >> sceneInfo->bckR >> sceneInfo->bckG >> sceneInfo->bckB;
        } else if(cmd == "DirectionalLight") {
            DirectionalLight* light = new DirectionalLight();
            file >> light->dx >> light->dy >> light->dz >> light->r >> light->g >> light->b;
            sceneInfo->lights[sceneInfo->numLights] = light;
            (sceneInfo->numLights)++;
        } else if(cmd == "Object") {
            std::string type;
            file >> type;

            if(type == "Sphere") {
                Sphere* sphere = new Sphere();
                Material* mat = new Material();
                file >> sphere->cx >> sphere->cy >> sphere->cz >> sphere->r;
                file >> mat->kd >> mat->ks >> mat->ka >> mat->odr >> mat->odg >> mat->odb >> mat->osr >> mat->osg >> mat->osb >> mat->kgls >> mat->refl >> mat->trans;

                sphere->material = mat;
                sceneInfo->sceneObjects[sceneInfo->numSceneObjects] = sphere;
                (sceneInfo->numSceneObjects)++;
            }
        }
    }
    file.close();

    sceneInfo->Print();
}