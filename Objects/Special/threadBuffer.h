#ifndef OBJECTS_SPECIAL_THREAD_BUFFER_H_
#define OBJECTS_SPECIAL_THREAD_BUFFER_H_

#include <atomic>

struct ThreadBuffer {
public:

    std::atomic<int> readIndex = -1;
    std::atomic<int> writeIndex = 0;
    std::atomic<bool> finished = false;
    unsigned char* volatile data = nullptr;
    std::atomic<bool>* volatile readiness = nullptr;

    int startX = 0;
    int startY = 0;
    int n = 0;

    ThreadBuffer(int startX, int startY, int n);
    ~ThreadBuffer();

};

#endif