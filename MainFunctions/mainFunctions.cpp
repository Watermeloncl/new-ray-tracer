#include <cfloat>
#include <cmath>

#include "mainFunctions.h"
#include "..\MathUtilities\mathUtilities.h"
#include "..\Objects\SceneObjects\sphere.h"
#include "..\Objects\Lights\directionalLight.h"
#include "..\Objects\Special\collisionInfo.h"
#include "..\Objects\Special\sceneInfo.h"
#include "..\config.h"

CollisionInfo* MainFunctions::FindCollision(double ox, double oy, double oz, double dx, double dy, double dz, SceneInfo* sceneInfo) {
    ox += (dx * RAY_EPSILON);
    oy += (dy * RAY_EPSILON);
    oz += (dz * RAY_EPSILON);

    //speed up by reducing multiplies?

    double lowestT = DBL_MAX;
    int oIndex = -1;

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

    double r = 0;
    double g = 0;
    double b = 0;

    GenericObject* o = sceneInfo->sceneObjects[collisionInfo->oIndex];
    if(o->type == ObjectType::SPHERE) {
        Sphere* sphere = (Sphere*)o;

        double nx = (collisionInfo->cpx - sphere->cx);
        double ny = (collisionInfo->cpy - sphere->cy);
        double nz = (collisionInfo->cpz - sphere->cz);
        //or divide each by r.
        MathUtilities::Normalize(nx, ny, nz);

        double dn = MathUtilities::DotProduct(dx, dy, dz, nx, ny, nz);
        double rx = dx - (2 * nx * dn);
        double ry = dy - (2 * ny * dn);
        double rz = dz - (2 * nz * dn);

        r += sphere->material->ka * sceneInfo->ambR * sphere->material->odr;
        g += sphere->material->ka * sceneInfo->ambG * sphere->material->odg;
        b += sphere->material->ka * sceneInfo->ambB * sphere->material->odb;

        for(int i = 0; i < sceneInfo->numLights; i++) {
            if(sceneInfo->lights[i]->type == LightType::DIRECTIONAL) {
                DirectionalLight* light = (DirectionalLight*)(sceneInfo->lights[i]);

                r += light->r * ((sphere->material->kd * sphere->material->odr * MathUtilities::DotProduct(nx, ny, nz, light->dx, light->dy, light->dz)) +
                                 (sphere->material->ks * sphere->material->osr * pow(MathUtilities::DotProduct(rx, ry, rz, -dx, -dy, -dz), sphere->material->kgls)));
                g += light->g * ((sphere->material->kd * sphere->material->odg * MathUtilities::DotProduct(nx, ny, nz, light->dx, light->dy, light->dz)) +
                                 (sphere->material->ks * sphere->material->osg * pow(MathUtilities::DotProduct(rx, ry, rz, -dx, -dy, -dz), sphere->material->kgls)));
                b += light->b * ((sphere->material->kd * sphere->material->odb * MathUtilities::DotProduct(nx, ny, nz, light->dx, light->dy, light->dz)) +
                                 (sphere->material->ks * sphere->material->osb * pow(MathUtilities::DotProduct(rx, ry, rz, -dx, -dy, -dz), sphere->material->kgls)));
            }
        }

        //r += reflection and transparency
    }
    
    colors->r = r;
    colors->g = g;
    colors->b = b;
    return colors;
}

double MainFunctions::CheckShadow(double ox, double oy, double oz, SceneInfo* sceneInfo) {
    return 0.0;
}