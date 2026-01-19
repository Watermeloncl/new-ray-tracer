#ifndef MAIN_FUNCTIONS_H_
#define MAIN_FUNCTIONS_H_

#include "..\Objects\Special\collisionInfo.h"
#include "..\Objects\Special\colorInfo.h"
#include "..\Objects\Special\sceneInfo.h"

class MainFunctions {
public:
    static CollisionInfo* FindCollision(double ox, double oy, double oz, double dx, double dy, double dz, SceneInfo* sceneInfo, int depth=0, bool earlyQuit=false);
    static ColorInfo* CalcColor(double dx, double dy, double dz, CollisionInfo* collisionInfo, SceneInfo* sceneInfo, int depth=0);
    static bool CheckInShadow(double ox, double oy, double oz, SceneInfo* sceneInfo, int lightIndex);
private:
    MainFunctions() = default;
};

#endif