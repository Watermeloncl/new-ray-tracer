#include <cfloat>
#include <cmath>
#include <iostream>
#include <stack>
#include <utility>

#include "mainFunctions.h"
#include "..\MathUtilities\mathUtilities.h"
#include "..\Objects\SceneObjects\sphere.h"
#include "..\Objects\SceneObjects\polygon.h"
#include "..\Objects\Lights\directionalLight.h"
#include "..\Objects\Lights\pointLight.h"
#include "..\Objects\Special\collisionInfo.h"
#include "..\Objects\Special\sceneInfo.h"
#include "..\config.h"

CollisionInfo* MainFunctions::FindCollision(double ox, double oy, double oz, double dx, double dy, double dz, SceneInfo* sceneInfo, double maxDist, int depth, bool earlyQuit) {
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

    double lowestT = maxDist;
    double t;
    int oIndex = -1;

    for(int i = 0; i < sceneInfo->numSceneObjects; i++) {
        if(sceneInfo->sceneObjects[i]->type == ObjectType::SPHERE) {
            double b, c, d, cx, cy, cz;

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

            if((t > 0) && (t < lowestT)) {
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
        } else if(sceneInfo->sceneObjects[i]->type == ObjectType::POLYGON) {
            Polygon* temp = (Polygon*)(sceneInfo->sceneObjects[i]);
            double d = -((temp->nx * temp->points[0]) + (temp->ny * temp->points[1]) + (temp->nz * temp->points[2]));

            t = (-((temp->nx * ox) + (temp->ny * oy) + (temp->nz * oz) + d)) / ((temp->nx * dx) + (temp->ny * dy) + (temp->nz * dz));
            if((t < 0) || (t > lowestT)) {
                continue;
            }

            double cpx = ox + (t * dx);
            double cpy = oy + (t * dy);
            double cpz = oz + (t * dz);

            double magx = fabs(temp->nx);
            double magy = fabs(temp->ny);
            double magz = fabs(temp->nz);

            int index1, index2;
            double p1, p2;

            if((magx >= magy) && (magx >= magz)) {
                index1 = 1;
                index2 = 2;
                p1 = cpy;
                p2 = cpz;
            } else if((magy >= magx) && (magy >= magz)) {
                index1 = 0;
                index2 = 2;
                p1 = cpx;
                p2 = cpz;
            } else {
                index1 = 0;
                index2 = 1;
                p1 = cpx;
                p2 = cpy;
            }

            bool inside = false;
            int j = temp->numPoints - 1;
            double x1, y1, x2, y2;

            for(int i = 0; i < temp->numPoints; i++) {
                x1 = temp->points[i*3 + index1];
                y1 = temp->points[i*3 + index2];
                x2 = temp->points[j*3 + index1];
                y2 = temp->points[j*3 + index2];

                if(((y1 > p2) != (y2 > p2)) &&
                   (p1 < (((x2 - x1) * (p2 - y1) / (y2 - y1)) + x1))) {
                    //(px < (xj - xi) * (py - yi) / (yj - yi) + xi)
                    inside = !inside;
                }


                j = i;
            }

            if(inside) {
                lowestT = t;
                oIndex = i;

                if(earlyQuit) {
                    break;
                }
            }
        }
    }

    if(lowestT == maxDist) {
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
    double ldx, ldy, ldz;
    for(int i = 0; i < sceneInfo->numLights; i++) {
        if(sceneInfo->lights[i]->type == LightType::DIRECTIONAL) {
            DirectionalLight* light = (DirectionalLight*)(sceneInfo->lights[i]);
            ldx = light->dx;
            ldy = light->dy;
            ldz = light->dz;

        } else if(sceneInfo->lights[i]->type == LightType::POINT) {
            PointLight* light = (PointLight*)(sceneInfo->lights[i]);
            ldx = collisionInfo->cpx - light->cx;
            ldy = collisionInfo->cpy - light->cy;
            ldz = collisionInfo->cpz - light->cz;

            MathUtilities::Normalize(ldx, ldy, ldz);
        } else {
            continue;
        }

        // do we back cull with refraction and transparency?
        nl = MathUtilities::DotProduct(nx, ny, nz, -ldx, -ldy, -ldz);
        if(nl < 0) {
            continue;
        }

        // how do we handle shadows with transparent and refractive materials?
        if(MainFunctions::CheckInShadow(collisionInfo->cpx, collisionInfo->cpy, collisionInfo->cpz, sceneInfo, i)) {
            continue;
        }

        rx = (2 * nl * nx) + ldx;
        ry = (2 * nl * ny) + ldy;
        rz = (2 * nl * nz) + ldz;
        
        rv = MathUtilities::DotProduct(rx, ry, rz, -dx, -dy, -dz);
        if(rv < 0) {
            rv = 0;
        } else {
            rv = pow(rv, mat->kgls);
        }

        r += sceneInfo->lights[i]->r * ((mat->kdodr * nl) +
                                        (mat->ksosr * rv));
        g += sceneInfo->lights[i]->g * ((mat->kdodg * nl) +
                                        (mat->ksosg * rv));
        b += sceneInfo->lights[i]->b * ((mat->kdodb * nl) +
                                        (mat->ksosb * rv));
    }

    double dn = MathUtilities::DotProduct(dx, dy, dz, nx, ny, nz);
    double rfx = dx - (2 * dn * nx);
    double rfy = dy - (2 * dn * ny);
    double rfz = dz - (2 * dn * nz);
    MathUtilities::Normalize(rfx, rfy, rfz);
    

    // Reflection
    if(mat->refl > 0) {
        CollisionInfo* reflectInfo = MainFunctions::FindCollision(collisionInfo->cpx, collisionInfo->cpy, collisionInfo->cpz, rfx, rfy, rfz, sceneInfo, DBL_MAX, depth + 1);
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

                CollisionInfo* refractInfo = MainFunctions::FindCollision(collisionInfo->cpx, collisionInfo->cpy, collisionInfo->cpz, tx, ty, tz, sceneInfo, DBL_MAX, depth + 1);
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

                CollisionInfo* refractInfo = MainFunctions::FindCollision(collisionInfo->cpx, collisionInfo->cpy, collisionInfo->cpz, tx, ty, tz, sceneInfo, DBL_MAX, depth + 1);
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

    double ldx, ldy, ldz, lightDist;
    if(sceneInfo->lights[lightIndex]->type == LightType::DIRECTIONAL) {
        DirectionalLight* light = (DirectionalLight*)(sceneInfo->lights[lightIndex]);

        ldx = light->dx;
        ldy = light->dy;
        ldz = light->dz;
        lightDist = DBL_MAX;

    } else if(sceneInfo->lights[lightIndex]->type == LightType::POINT) {
        PointLight* light = (PointLight*)(sceneInfo->lights[lightIndex]);

        ldx = ox - light->cx;
        ldy = oy - light->cy;
        ldz = oz - light->cz;

        lightDist = std::sqrt((ldx*ldx) + (ldy*ldy) + (ldz*ldz));
        double invMag = 1 / lightDist;

        ldx *= invMag;
        ldy *= invMag;
        ldz *= invMag;
    } else {
        return false;
    }

    CollisionInfo* info = MainFunctions::FindCollision(ox, oy, oz, -ldx, -ldy, -ldz, sceneInfo, lightDist, 0, true);

    if(info == nullptr) {
        return false;
    } else {
        delete info;
        return true;
    }
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
