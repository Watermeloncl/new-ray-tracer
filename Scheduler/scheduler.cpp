#include <windows.h>
#include <chrono>
#include <iostream>

#include "scheduler.h"
#include "..\Display\display.h"
#include "..\config.h"

using SchedClock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double>;
using TimeStamp = std::chrono::time_point<std::chrono::high_resolution_clock>;

SchedulerModule::SchedulerModule(HINSTANCE hInstance, int nCmdShow) {
    this->display = new DisplayModule(hInstance, nCmdShow);
}

SchedulerModule::~SchedulerModule() {
    delete this->display;   
}

void SchedulerModule::RunMainLoop() {
    TimeStamp lastTime = SchedClock::now();
    TimeStamp now = SchedClock::now();

    Duration elapsed;
    double accumulator = 0.0;
    const double dt = 1.0 / 60.0;

    UINT r, g, b;
    int frame = 0;
    bool running = true;

    MSG msg = {};
    while(running) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) running = false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        now = SchedClock::now();
        elapsed = now - lastTime;
        lastTime = now;
        accumulator += elapsed.count();
 
        while(accumulator >= dt) {
            frame++;

            for(int y = 0; y < CLIENT_SCREEN_HEIGHT; y++) {
                for (int x = 0; x < CLIENT_SCREEN_WIDTH; x++) {
                    r = (x + frame) % 256;
                    g = (y + frame) % 256;
                    b = (x + y + frame) % 256;
                    this->screenPixels[y * CLIENT_SCREEN_WIDTH + x] = 0xFF000000 | (r << 16) | (g << 8) | b;
                }
            }

            accumulator -= dt;
        }

        this->display->RenderFrame(this->screenPixels);
    }


}