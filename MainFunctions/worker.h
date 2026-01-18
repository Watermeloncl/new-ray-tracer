#ifndef MAIN_FUNCTIONS_WORKER_H_
#define MAIN_FUNCTIONS_WORKER_H_

#include "..\Objects\Special\threadBuffer.h"

class Worker {
public:
    static void ComputePixels(ThreadBuffer* buffer);
private:
    Worker() = default;

    static void SignalReady(ThreadBuffer* buffer);
};

#endif