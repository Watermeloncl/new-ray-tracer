#include <windows.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "scheduler.h"
#include "..\Display\display.h"
#include "..\Resources\parser.h"
#include "..\Objects\Special\threadBuffer.h"
#include "..\Objects\Special\sceneInfo.h"
#include "..\MainFunctions\worker.h"
#include "..\config.h"

using SchedClock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double>;
using TimeStamp = std::chrono::time_point<std::chrono::high_resolution_clock>;

SchedulerModule::SchedulerModule(HINSTANCE hInstance, int nCmdShow) {
    this->display = new DisplayModule(hInstance, nCmdShow);
    this->sceneInfo = new SceneInfo();
    this->parser = new Parser();
}

SchedulerModule::~SchedulerModule() {
    delete this->display;   
    delete[] this->buffers;

    for(std::thread &t : workers) {
        if(t.joinable()) {
            t.join();
        }
    }
}

void SchedulerModule::RunMainLoop() {
    this->PrepScreen();
    this->InitThreads();
    this->parser->ParseInput(this->sceneInfo);

    TimeStamp lastTime = SchedClock::now();
    TimeStamp now = SchedClock::now();

    Duration elapsed;
    double accumulator = 0.0;
    const double dt = 1.0 / 60.0;

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

            for(int i = 0; i < NUM_WORKER_THREADS; i++) {
                this->AddPixels(this->buffers[i], i);
            }

            this->CheckToWrite();

            accumulator -= dt;
        }

        this->display->RenderFrame(this->screenPixels);
    }
}

void SchedulerModule::PrepScreen() {
    int width, height;

    if(SHOW_ON_SCREEN_FLAG) {
        width = CLIENT_SCREEN_WIDTH;
        height = CLIENT_SCREEN_HEIGHT;
    } else {
        width = MIN_CLIENT_SCREEN_WIDTH;
        height = MIN_CLIENT_SCREEN_HEIGHT;
    }

    this->screenPixels = new UINT32[width * height];
    this->filePixels = new unsigned char[CLIENT_SCREEN_WIDTH * CLIENT_SCREEN_HEIGHT * 3];

    for(int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            this->screenPixels[y * width + x] = 0xFF000000;
        }
    }
}

void SchedulerModule::InitThreads() {
    this->buffers = new ThreadBuffer*[NUM_WORKER_THREADS];

    int totalPixels = CLIENT_SCREEN_WIDTH * CLIENT_SCREEN_HEIGHT;
    int n = totalPixels / NUM_WORKER_THREADS;
    int startY;

    for(int i = 0; i < NUM_WORKER_THREADS; i++) {
        startY = (n * i) / CLIENT_SCREEN_WIDTH;
        ThreadBuffer* buffer = new ThreadBuffer(0, startY, n);
        this->buffers[i] = buffer;

        this->workers.push_back(std::thread(Worker::ComputePixels, buffer));
    }
}

void SchedulerModule::AddPixels(ThreadBuffer* buffer, int threadNum) {
    int destinationIndex = buffer->startY * CLIENT_SCREEN_WIDTH;
    int dest3Index = destinationIndex * 3;
    int sourceIndex = buffer->readIndex + 1;

    while(buffer->readiness[sourceIndex]) {
        if(SHOW_ON_SCREEN_FLAG) {
            this->screenPixels[destinationIndex] = 0xFF000000 | (((UINT32)(buffer->data[sourceIndex])) << 16) | (((UINT32)(buffer->data[sourceIndex + 1])) << 8) | ((UINT32)(buffer->data[sourceIndex + 2]));
        }

        if(OUT_TO_FILE_FLAG) {
            this->filePixels[dest3Index] = buffer->data[sourceIndex];
            this->filePixels[dest3Index + 1] = buffer->data[sourceIndex + 1];
            this->filePixels[dest3Index + 2] = buffer->data[sourceIndex + 2];
        }

        destinationIndex++;
        sourceIndex += 3;
        dest3Index += 3;
    }

}

void SchedulerModule::CheckToWrite() {
    bool finished = true;
    for(int i = 0; i < NUM_WORKER_THREADS; i++) {
        if(!(this->buffers[i]->finished)) {
            finished = false;
            break;
        }
    }

    if(finished && !(this->writtenOut) && OUT_TO_FILE_FLAG) {
        FILE* fp = fopen(OUT_FILE_NAME, "wb");
        if(!fp) {
            std::cout << "No file found!" << std::endl;
        }

        fprintf(fp, "P6\n%d %d\n255\n", CLIENT_SCREEN_WIDTH, CLIENT_SCREEN_HEIGHT);
        fwrite(this->filePixels, 1, (size_t)CLIENT_SCREEN_WIDTH * CLIENT_SCREEN_HEIGHT * 3, fp);

        fclose(fp);
    }
}