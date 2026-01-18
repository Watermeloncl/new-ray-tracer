#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <windows.h>
#include <vector>
#include <thread>

#include "..\Display\display.h"
#include "..\Objects\threadBuffer.h"
#include "..\config.h"

class SchedulerModule {
public:
private:
    DisplayModule* display = nullptr;

    UINT32* screenPixels = nullptr;
    unsigned char* filePixels = nullptr;

    std::vector<std::thread> workers;
    ThreadBuffer** buffers = nullptr;

    bool writtenOut = false;
public:
    SchedulerModule(HINSTANCE hInstance, int nCmdShow);
    ~SchedulerModule();

    void RunMainLoop();

private:
    void PrepScreen();
    void InitThreads();

    void AddPixels(ThreadBuffer* buffer, int threadNum);

    void CheckToWrite();
};

#endif