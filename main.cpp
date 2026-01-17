#include <iostream>
#include <windows.h>
#include <chrono>
#include <dwrite.h>
#include <d2d1.h>
#include <fstream>
#include <cstdint>

#include "Scheduler\scheduler.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SchedulerModule* sched = new SchedulerModule(hInstance, nCmdShow);
    sched->RunMainLoop();
    return 0;
}