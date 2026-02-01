#ifndef MAIN_FUNCTIONS_H_
#define MAIN_FUNCTIONS_H_

#include <random>

#include "..\Objects\Special\collisionInfo.h"
#include "..\Objects\Special\colorInfo.h"
#include "..\Objects\Special\sceneInfo.h"
#include "..\Objects\Special\boundingBox.h"

class MainFunctions {
private:
    static thread_local std::mt19937 gen;
    static thread_local std::normal_distribution<double> glossDistro;
    static thread_local std::normal_distribution<double> transDistro;
public:
    static CollisionInfo* FindCollision(double ox, double oy, double oz, double dx, double dy, double dz, SceneInfo* sceneInfo, std::stack<double>* nitStack, double maxDist, int depth=0, bool earlyQuit=false);
    static double CollidesWithBox(BoundingBox* box, double ox, double oy, double oz, double dx, double dy, double dz);
    static ColorInfo* CalcColor(double dx, double dy, double dz, CollisionInfo* collisionInfo, SceneInfo* sceneInfo, std::stack<double>* nitStack, int depth=0);
    static bool CheckInShadow(double ox, double oy, double oz, SceneInfo* sceneInfo, std::stack<double>* nitStack, int lightIndex);

    static void InitStartingStack(SceneInfo* sceneInfo);
private:
    MainFunctions() = default;
};

#endif