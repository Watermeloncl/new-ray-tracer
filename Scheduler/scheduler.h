#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <windows.h>

#include "..\Display\display.h"
#include "..\config.h"

class SchedulerModule {
public:
private:
    DisplayModule* display = nullptr;

    UINT32* screenPixels = new UINT32[CLIENT_SCREEN_WIDTH * CLIENT_SCREEN_HEIGHT];
public:
    SchedulerModule(HINSTANCE hInstance, int nCmdShow);
    ~SchedulerModule();

    void RunMainLoop();

private:
};

#endif