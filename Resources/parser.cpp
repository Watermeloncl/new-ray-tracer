#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cfloat>
#include <vector>

#include "parser.h"
#include "..\Objects\Special\sceneInfo.h"
#include "..\Objects\Special\boundingBox.h"
#include "..\Objects\Lights\directionalLight.h"
#include "..\Objects\Lights\pointLight.h"
#include "..\Objects\SceneObjects\sphere.h"
#include "..\Objects\SceneObjects\polygon.h"
#include "..\Objects\SceneObjects\material.h"
#include "..\MathUtilities\mathUtilities.h"
#include "..\MainFunctions\mainFunctions.h"
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
        } else if(cmd == "Light") {
            std::string type;
            file >> type;

            if(type == "Directional") {
                DirectionalLight* light = new DirectionalLight();
                file >> light->dx >> light->dy >> light->dz >> light->r >> light->g >> light->b;
                MathUtilities::Normalize(light->dx, light->dy, light->dz);
                sceneInfo->lights[sceneInfo->numLights] = light;
            } else if(type == "Point") {
                PointLight* light = new PointLight();
                file >> light->cx >> light->cy >> light->cz >> light->r >> light->g >> light->b;
                sceneInfo->lights[sceneInfo->numLights] = light;
            }

            (sceneInfo->numLights)++;
        } else if(cmd == "Object") {
            std::string type;
            file >> type;
            Material* mat = new Material();

            if(type == "Sphere") {
                Sphere* sphere = new Sphere(sceneInfo->numSceneObjects);
                sphere->material = mat;

                file >> sphere->cx >> sphere->cy >> sphere->cz >> sphere->r;
                if(mat->nit <= 0) {
                    std::cout << "nit 0 or below not allowed!" << std::endl;
                    exit(1);
                }

                sceneInfo->sceneObjects[sceneInfo->numSceneObjects] = sphere;
            } else if(type == "Polygon") {
                Polygon* polygon = new Polygon(sceneInfo->numSceneObjects);
                polygon->material = mat;

                file >> polygon->numPoints;
                polygon->points = new double[polygon->numPoints * 3];
                for(int i = 0; i < (polygon->numPoints * 3); i += 3) {
                    file >> polygon->points[i] >> polygon->points[i + 1] >> polygon->points[i + 2];
                }

                sceneInfo->sceneObjects[sceneInfo->numSceneObjects] = polygon;
            }

            (sceneInfo->numSceneObjects)++;
            file >> mat->kd >> mat->ks >> mat->ka >> mat->odr >> mat->odg >> mat->odb >> mat->osr >> mat->osg >> mat->osb >> mat->kgls >> mat->refl >> mat->trans >> mat->nit;
        }
    }
    file.close();

    this->AddPrecomputes(sceneInfo);
    this->CreateTreeStructure(sceneInfo);
    MainFunctions::InitStartingStack(sceneInfo);
    // sceneInfo->Print();
    // sceneInfo->PrintTree();
}

void Parser::AddPrecomputes(SceneInfo* sceneInfo) {
    Material* temp;

    for(int i = 0; i < sceneInfo->numSceneObjects; i++) {
        temp = sceneInfo->sceneObjects[i]->material;
        temp->ar = sceneInfo->ambR * temp->ka * temp->odr;
        temp->ag = sceneInfo->ambG * temp->ka * temp->odg;
        temp->ab = sceneInfo->ambB * temp->ka * temp->odb;
        temp->kdodr = temp->kd * temp->odr;
        temp->kdodg = temp->kd * temp->odg;
        temp->kdodb = temp->kd * temp->odb;
        temp->ksosr = temp->ks * temp->osr;
        temp->ksosg = temp->ks * temp->osg;
        temp->ksosb = temp->ks * temp->osb;
    }

    Sphere* sphere;
    Polygon* polygon;

    for(int i = 0; i < sceneInfo->numSceneObjects; i++) {
        if(sceneInfo->sceneObjects[i]->type == ObjectType::SPHERE) {
            sphere = (Sphere*)(sceneInfo->sceneObjects[i]);

            sphere->cxcx = (sphere->cx)*(sphere->cx);
            sphere->cycy = (sphere->cy)*(sphere->cy);
            sphere->czcz = (sphere->cz)*(sphere->cz);
            sphere->rr = (sphere->r)*(sphere->r);
        } else if(sceneInfo->sceneObjects[i]->type == ObjectType::POLYGON) {
            polygon = (Polygon*)(sceneInfo->sceneObjects[i]);

            double x1 = polygon->points[3] - polygon->points[0];
            double y1 = polygon->points[4] - polygon->points[1];
            double z1 = polygon->points[5] - polygon->points[2];

            double x2 = polygon->points[6] - polygon->points[3];
            double y2 = polygon->points[7] - polygon->points[4];
            double z2 = polygon->points[8] - polygon->points[5];

            polygon->nx = (y1*z2) - (z1*y2);
            polygon->ny = -((x1*z2) - (z1*x2));
            polygon->nz = (x1*y2) - (y1*x2);

            std::cout << polygon->nx << " " << polygon->ny << " " << polygon->nz << std::endl;
            MathUtilities::Normalize(polygon->nx, polygon->ny, polygon->nz);
        }
    }

}

void Parser::CreateTreeStructure(SceneInfo* sceneInfo) {
    double highX = -DBL_MAX;
    double highY = -DBL_MAX;
    double highZ = -DBL_MAX;

    double lowX = DBL_MAX;
    double lowY = DBL_MAX;
    double lowZ = DBL_MAX;

    GenericObject* currObject;
    for(int i = 0; i < sceneInfo->numSceneObjects; i++) {
        currObject = sceneInfo->sceneObjects[i];
        currObject->BuildMiniBox();

        highX = std::max(highX, currObject->highX);
        highY = std::max(highY, currObject->highY);
        highZ = std::max(highZ, currObject->highZ);

        lowX = std::min(lowX, currObject->lowX);
        lowY = std::min(lowY, currObject->lowY);
        lowZ = std::min(lowZ, currObject->lowZ);
    }

    BoundingBox* headBox = new BoundingBox();
    headBox->lowX = lowX;
    headBox->lowY = lowY;
    headBox->lowZ = lowZ;

    headBox->highX = highX;
    headBox->highY = highY;
    headBox->highZ = highZ;

    std::vector<GenericObject*> objects;
    objects.reserve(sceneInfo->numSceneObjects);
    for(int i = 0; i < sceneInfo->numSceneObjects; i++) {
        objects.push_back(sceneInfo->sceneObjects[i]);
    }

    headBox->SplitCreate(objects);

    sceneInfo->headBox = headBox;
}