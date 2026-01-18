#include <cfloat>
#include <cmath>

#include "mainFunctions.h"
#include "..\Objects\SceneObjects\sphere.h"
#include "..\Objects\Special\collisionInfo.h"
#include "..\Objects\Special\sceneInfo.h"
#include "..\config.h"

CollisionInfo* MainFunctions::FindCollision(double ox, double oy, double oz, double dx, double dy, double dz, SceneInfo* sceneInfo) {
    ox += (dx * RAY_EPSILON);
    oy += (dy * RAY_EPSILON);
    oz += (dz * RAY_EPSILON);

    //speed up by reducing multiplies?

    double lowestT = DBL_MAX;
    double oIndex = -1;

    for(int i = 0; i < sceneInfo->numSceneObjects; i++) {
        if(sceneInfo->sceneObjects[i]->type == ObjectType::SPHERE) {
            double b, c, d, cx, cy, cz, r, t;

            Sphere* temp = (Sphere*)(sceneInfo->sceneObjects[i]);
            cx = temp->cx;
            cy = temp->cy;
            cz = temp->cz;
            r = temp->r;
            b = 2 * ((dx*ox) - (dx*cx) + (dy*oy) - (dy*cy) + (dz*oz) - (dz*cz));
            c = (ox*ox) - (2*ox*cx) + (cx*cx) + (oy*oy) - (2*oy*cy) + (cy*cy) + (oz*oz) - (2*oz*cz) + (cz*cz) - (r*r);

            d = (b*b) - (4*c);
            if(d < 0) {
                continue;
            }

            d = std::sqrt(d);
            t = (-b - d) / 2;
            if(lowestT < t) {
                continue;
            }

            if(t > 0) {
                lowestT = t;
                oIndex = i;
                continue;
            } else {
                t = (-b + d) / 2;
                if((t > 0) && (t < lowestT)) {
                    lowestT = t;
                    oIndex = i;
                }
            }
        }
    }

    if(lowestT == DBL_MAX) {
        return nullptr;
    }

    CollisionInfo* info = new CollisionInfo();
    info->t = lowestT;
    info->oIndex = oIndex;
    info->cpx = ox + (lowestT * dx);
    info->cpy = oy + (lowestT * dy);
    info->cpz = oz + (lowestT * dz);

    return info;
}

ColorInfo* MainFunctions::CalcColor(double dx, double dy, double dz, CollisionInfo* collisionInfo, SceneInfo* sceneInfo) {
    ColorInfo* colors = new ColorInfo();

    if(collisionInfo == nullptr) {
        colors->r = sceneInfo->bckR;
        colors->g = sceneInfo->bckG;
        colors->b = sceneInfo->bckB;
        return colors;
    }

    
    
    colors->r = 0;
    colors->g = 0.5490196;
    colors->b = 0;
    return colors;
}

double MainFunctions::CheckShadow(double ox, double oy, double oz, SceneInfo* sceneInfo) {
    return 0.0;
}