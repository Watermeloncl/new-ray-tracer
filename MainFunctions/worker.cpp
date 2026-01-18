#include <iostream>
#include <chrono>
#include <thread>

#include "worker.h"
#include "..\Objects\Special\threadBuffer.h"

void Worker::ComputePixels(ThreadBuffer* buffer) {
    for(int i = 0; i < buffer->n; i++) {




        buffer->data[buffer->writeIndex] = (unsigned char)0;
        buffer->data[buffer->writeIndex + 1] = (unsigned char)140;
        buffer->data[buffer->writeIndex + 2] = (unsigned char)0;
        Worker::SignalReady(buffer);
    }

    buffer->finished = true;
}

void Worker::SignalReady(ThreadBuffer* buffer) {
    buffer->readiness[buffer->writeIndex + 2] = true;
    buffer->readiness[buffer->writeIndex + 1] = true;
    buffer->writeIndex += 3;

    buffer->readiness[buffer->writeIndex - 3] = true;
}