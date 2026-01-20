#include <cfloat>
#include <cmath>
#include <iostream>
#include <stack>

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

ColorInfo* MainFunctions::CalcColor(double dx, double dy, double dz, CollisionInfo* collisionInfo, SceneInfo* sceneInfo, std::stack<double>* nitStack, int depth) {
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

    GenericObject* collided = sceneInfo->sceneObjects[collisionInfo->oIndex];
    Material* mat = collided->material;

    double nx = 0;
    double ny = 0;
    double nz = 0;
    collided->CalcNormal(collisionInfo->cpx, collisionInfo->cpy, collisionInfo->cpz, nx, ny, nz);

    r += mat->ar;
    g += mat->ag;
    b += mat->ab;

    double rx, ry, rz, nl, rv;

    for(int i = 0; i < sceneInfo->numLights; i++) {
        if(sceneInfo->lights[i]->type == LightType::DIRECTIONAL) {
            DirectionalLight* light = (DirectionalLight*)(sceneInfo->lights[i]);

            nl = MathUtilities::DotProduct(nx, ny, nz, -light->dx, -light->dy, -light->dz);
            if(nl < 0) {
                continue;
            }

            if(MainFunctions::CheckInShadow(collisionInfo->cpx, collisionInfo->cpy, collisionInfo->cpz, sceneInfo, i)) {
                continue;
            }

            rx = (2 * nl * nx) + light->dx;
            ry = (2 * nl * ny) + light->dy;
            rz = (2 * nl * nz) + light->dz;
            
            rv = MathUtilities::DotProduct(rx, ry, rz, -dx, -dy, -dz);
            if(rv < 0) {
                rv = 0;
            } else {
                rv = pow(rv, mat->kgls);
            }

            r += light->r * ((mat->kdodr * nl) +
                                (mat->ksosr * rv));
            g += light->g * ((mat->kdodg * nl) +
                                (mat->ksosg * rv));
            b += light->b * ((mat->kdodb * nl) +
                                (mat->ksosb * rv));
        }
    }

    double dn = MathUtilities::DotProduct(dx, dy, dz, nx, ny, nz);
    double rfx = dx - (2 * dn * nx);
    double rfy = dy - (2 * dn * ny);
    double rfz = dz - (2 * dn * nz);
    MathUtilities::Normalize(rfx, rfy, rfz);
    

    // Reflection
    if(mat->refl > 0) {
        CollisionInfo* reflectInfo = MainFunctions::FindCollision(collisionInfo->cpx, collisionInfo->cpy, collisionInfo->cpz, rfx, rfy, rfz, sceneInfo, depth + 1);
        ColorInfo* reflectColor = MainFunctions::CalcColor(rfx, rfy, rfz, reflectInfo, sceneInfo, nitStack, depth + 1);
        r += (reflectColor->r)*(mat->refl);
        g += (reflectColor->g)*(mat->refl);
        b += (reflectColor->b)*(mat->refl);
        delete reflectColor;
        delete reflectInfo;
    }


    // Refraction
    if(mat->trans > 0) {
        double cosTheta = -dn;
        if(dn < 0) {
            //outside polygon/sphere
            double nit = nitStack->top() / mat->nit;
            double d = 1 + (nit*nit)*((cosTheta*cosTheta) - 1);
            if((nit < 1) || (d >= 0)) {
                //no TIR (total internal refraction)
                nitStack->push(mat->nit);

                double inner = (nit*cosTheta) - std::sqrt(d);
                double tx = (nit*dx) + (inner*nx);
                double ty = (nit*dy) + (inner*ny);
                double tz = (nit*dz) + (inner*nz);
                MathUtilities::Normalize(tx, ty, tz);

                CollisionInfo* refractInfo = MainFunctions::FindCollision(collisionInfo->cpx, collisionInfo->cpy, collisionInfo->cpz, tx, ty, tz, sceneInfo, depth + 1);
                ColorInfo* refractColor = MainFunctions::CalcColor(tx, ty, tz, refractInfo, sceneInfo, nitStack, depth + 1);

                nitStack->pop();

                r += (refractColor->r)*(mat->trans);
                g += (refractColor->g)*(mat->trans);
                b += (refractColor->b)*(mat->trans);
                delete refractInfo;
                delete refractColor;
            }
        } else {
            //inside
            double current = nitStack->top();
            nitStack->pop();
            double next = nitStack->top();

            double nit = current / next;
            cosTheta *= -1;
            double d = 1 + (nit*nit)*((cosTheta*cosTheta) - 1);
            if((nit < 1) || (d >= 0)) {
                double inner = (nit*cosTheta) - std::sqrt(d);
                double tx = (nit*dx) + (inner*(-nx));
                double ty = (nit*dy) + (inner*(-ny));
                double tz = (nit*dz) + (inner*(-nz));

                CollisionInfo* refractInfo = MainFunctions::FindCollision(collisionInfo->cpx, collisionInfo->cpy, collisionInfo->cpz, tx, ty, tz, sceneInfo, depth + 1);
                ColorInfo* refractColor = MainFunctions::CalcColor(tx, ty, tz, refractInfo, sceneInfo, nitStack, depth + 1);

                r += (refractColor->r)*(mat->trans);
                g += (refractColor->g)*(mat->trans);
                b += (refractColor->b)*(mat->trans);
                delete refractInfo;
                delete refractColor;
            }
            
            nitStack->push(current);
        }
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

// Only accounts for spheres, and can only start within one.
// I didn't want to spend the time working on more. (like overlapping or polygons!)
void MainFunctions::InitStartingStack(SceneInfo* sceneInfo) {
    sceneInfo->startingStack = new std::stack<double>();
    sceneInfo->startingStack->push(1.0);

    double minT = DBL_MAX;
    double startNit = 1.0;
    
    double deltaX, deltaY, deltaZ;
    for(int i = 0; i < sceneInfo->numSceneObjects; i++) {
        if(sceneInfo->sceneObjects[i]->type == ObjectType::SPHERE) {
            Sphere* temp = (Sphere*)(sceneInfo->sceneObjects[i]);
            deltaX = temp->cx;
            deltaY = temp->cy;
            deltaZ = temp->cz + sceneInfo->viewDistance;

            double dist = (deltaX*deltaX) + (deltaY*deltaY) + (deltaZ*deltaZ);

            if((dist < temp->rr) && (dist < minT)) {
                minT = dist;
                startNit = temp->material->nit;
            }
        }
    }

    if(minT < DBL_MAX) {
        sceneInfo->startingStack->push(startNit);
    }
}
