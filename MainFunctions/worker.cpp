#include <iostream>
#include <chrono>
#include <thread>
#include <cfloat>

#include "worker.h"
#include "..\Objects\Special\threadBuffer.h"
#include "..\Objects\Special\collisionInfo.h"
#include "..\Objects\Special\colorInfo.h"
#include "mainFunctions.h"
#include "..\MathUtilities\mathUtilities.h"
#include "..\config.h"

void Worker::ComputePixels(ThreadBuffer* buffer) {
    SceneInfo* sceneInfo = buffer->sceneInfo;
    double ox = 0;
    double oy = 0;
    double oz = -sceneInfo->viewDistance;

    double dx = 0;
    double dy = 0;
    double dz = sceneInfo->viewDistance;

    double leftX = (sceneInfo->worldSpace * CLIENT_WORLD_RATIO_WIDTH) / -2.0;
    double rightX = (sceneInfo->worldSpace * CLIENT_WORLD_RATIO_WIDTH) / 2.0;
    double upY = (sceneInfo->worldSpace * CLIENT_WORLD_RATIO_HEIGHT) / 2.0;

    double pixelSize = (sceneInfo->worldSpace * CLIENT_WORLD_RATIO_WIDTH) / CLIENT_SCREEN_WIDTH;

    double currX = (buffer->startX * pixelSize) + leftX + (pixelSize / 2.0);
    double currY = upY - (buffer->startY * pixelSize) - (pixelSize / 2.0);

    double r, g, b;
    double invTotColor;

    // double subX, subY;

    for(int i = 0; i < buffer->n; i++) {
        r = 0;
        g = 0;
        b = 0;

        // for(;;) {
            dx = currX;
            dy = currY;
            dz = sceneInfo->viewDistance;

            MathUtilities::Normalize(dx, dy, dz);
            CollisionInfo* collisionInfo = MainFunctions::FindCollision(ox, oy, oz, dx, dy, dz, sceneInfo, DBL_MAX);
            ColorInfo* colorInfo = MainFunctions::CalcColor(dx, dy, dz, collisionInfo, sceneInfo, buffer->threadStartNitStack);

            r += colorInfo->r;
            g += colorInfo->g;
            b += colorInfo->b;

            delete collisionInfo;
            delete colorInfo;
        // }

        invTotColor = 1 / (r + g + b);
        r *= invTotColor;
        g *= invTotColor;
        b *= invTotColor;


        buffer->data[buffer->writeIndex] = MathUtilities::ColorAmp(r);
        buffer->data[buffer->writeIndex + 1] = MathUtilities::ColorAmp(g);
        buffer->data[buffer->writeIndex + 2] = MathUtilities::ColorAmp(b);

        Worker::SignalReady(buffer);

        currX += pixelSize;
        if(currX > rightX) {
            currX = leftX + (pixelSize / 2.0);
            currY -= pixelSize;
        }
    }

    buffer->finished = true;
}

void Worker::SignalReady(ThreadBuffer* buffer) {
    buffer->readiness[buffer->writeIndex + 2] = true;
    buffer->readiness[buffer->writeIndex + 1] = true;
    buffer->writeIndex += 3;

    buffer->readiness[buffer->writeIndex - 3] = true;
}