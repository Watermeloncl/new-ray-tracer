#include <cfloat>
#include <cmath>
#include <iostream>

#include "mainFunctions.h"
#include "..\MathUtilities\mathUtilities.h"
#include "..\Objects\SceneObjects\sphere.h"
#include "..\Objects\Lights\directionalLight.h"
#include "..\Objects\Special\collisionInfo.h"
#include "..\Objects\Special\sceneInfo.h"
#include "..\config.h"

CollisionInfo* MainFunctions::FindCollision(double ox, double oy, double oz, double dx, double dy, double dz, SceneInfo* sceneInfo, int depth, bool earlyQuit) {
    if(!earlyQuit && (depth > MAX_RAY_DEPTH)) {
        return nullptr;
    }

    ox += (dx * RAY_EPSILON);
    oy += (dy * RAY_EPSILON);
    oz += (dz * RAY_EPSILON);

    // Speed up values
    double oxox = ox*ox;
    double oyoy = oy*oy;
    double ozoz = oz*oz;
    double dxox = dx*ox;
    double dyoy = dy*oy;
    double dzoz = dz*oz;
    double ox2 = ox*2;
    double oy2 = oy*2;
    double oz2 = oz*2;

    double lowestT = DBL_MAX;
    int oIndex = -1;

    for(int i = 0; i < sceneInfo->numSceneObjects; i++) {
        if(sceneInfo->sceneObjects[i]->type == ObjectType::SPHERE) {
            double b, c, d, cx, cy, cz, t;

            Sphere* temp = (Sphere*)(sceneInfo->sceneObjects[i]);
            cx = temp->cx;
            cy = temp->cy;
            cz = temp->cz;
            b = 2 * ((dxox) - (dx*cx) + (dyoy) - (dy*cy) + (dzoz) - (dz*cz));
            c = (oxox) - (ox2*cx) + (temp->cxcx) + (oyoy) - (oy2*cy) + (temp->cycy) + (ozoz) - (oz2*cz) + (temp->czcz) - (temp->rr);

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

                if(earlyQuit) {
                    break;
                }

                continue;
            } else {
                t = (-b + d) / 2;
                if((t > 0) && (t < lowestT)) {
                    lowestT = t;
                    oIndex = i;

                    if(earlyQuit) {
                        break;
                    }
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

ColorInfo* MainFunctions::CalcColor(double dx, double dy, double dz, CollisionInfo* collisionInfo, SceneInfo* sceneInfo, int depth) {
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

        double invr = 1 / sphere->r;
        double nx = (collisionInfo->cpx - sphere->cx) * invr;
        double ny = (collisionInfo->cpy - sphere->cy) * invr;
        double nz = (collisionInfo->cpz - sphere->cz) * invr;

        r += sphere->material->ar;
        g += sphere->material->ag;
        b += sphere->material->ab;

        for(int i = 0; i < sceneInfo->numLights; i++) {
            if(sceneInfo->lights[i]->type == LightType::DIRECTIONAL) {
                DirectionalLight* light = (DirectionalLight*)(sceneInfo->lights[i]);

                double nl = MathUtilities::DotProduct(nx, ny, nz, -light->dx, -light->dy, -light->dz);
                if(nl < 0) {
                    continue;
                }

                if(MainFunctions::CheckInShadow(collisionInfo->cpx, collisionInfo->cpy, collisionInfo->cpz, sceneInfo, i)) {
                    continue;
                }

                double rx = (2 * nl * nx) + light->dx;
                double ry = (2 * nl * ny) + light->dy;
                double rz = (2 * nl * nz) + light->dz;

                
                double rv = MathUtilities::DotProduct(rx, ry, rz, -dx, -dy, -dz);
                if(rv < 0) {
                    rv = 0;
                } else {
                    rv = pow(rv, sphere->material->kgls);
                }

                r += light->r * ((sphere->material->kdodr * nl) +
                                 (sphere->material->ksosr * rv));
                g += light->g * ((sphere->material->kdodg * nl) +
                                 (sphere->material->ksosg * rv));
                b += light->b * ((sphere->material->kdodb * nl) +
                                 (sphere->material->ksosb * rv));
            }
        }

        double dn = MathUtilities::DotProduct(dx, dy, dz, nx, ny, nz);
        double rfx = dx - (2 * dn * nx);
        double rfy = dy - (2 * dn * ny);
        double rfz = dz - (2 * dn * nz);

        if(sphere->material->refl > 0) {
            CollisionInfo* reflectInfo = MainFunctions::FindCollision(collisionInfo->cpx, collisionInfo->cpy, collisionInfo->cpz, rfx, rfy, rfz, sceneInfo, depth + 1);
            ColorInfo* reflectColor = MainFunctions::CalcColor(rfx, rfy, rfz, reflectInfo, sceneInfo, depth + 1);
            r += (reflectColor->r)*(sphere->material->refl);
            g += (reflectColor->g)*(sphere->material->refl);
            b += (reflectColor->b)*(sphere->material->refl);
            delete reflectColor;
            delete reflectInfo;
        }

        //r += transparency
        

    }
    
    colors->r = r;
    colors->g = g;
    colors->b = b;

    return colors;
}

bool MainFunctions::CheckInShadow(double ox, double oy, double oz, SceneInfo* sceneInfo, int lightIndex) {
    CollisionInfo* info;

    if(sceneInfo->lights[lightIndex]->type == LightType::DIRECTIONAL) {
        DirectionalLight* light = (DirectionalLight*)(sceneInfo->lights[lightIndex]);

        info = MainFunctions::FindCollision(ox, oy, oz, -(light->dx), -(light->dy), -(light->dz), sceneInfo, true);

        if(info == nullptr) {
            return false;
        } else {
            delete info;
            return true;
        }
    }

    return 0.0;
}