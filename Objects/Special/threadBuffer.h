#ifndef OBJECTS_SPECIAL_THREAD_BUFFER_H_
#define OBJECTS_SPECIAL_THREAD_BUFFER_H_

#include <atomic>
#include <stack>

#include "sceneInfo.h"

struct ThreadBuffer {
public:

    std::atomic<int> readIndex = -1;
    std::atomic<int> writeIndex = 0;
    std::atomic<bool> finished = false;
    unsigned char* volatile data = nullptr;
    std::atomic<bool>* volatile readiness = nullptr;

    std::stack<double>* threadStartNitStack = nullptr;

    int startX = 0;
    int startY = 0;
    int n = 0;

    SceneInfo* sceneInfo = nullptr;

    ThreadBuffer(int startX, int startY, int n, SceneInfo* sceneInfo);
    ~ThreadBuffer();
};

#endif