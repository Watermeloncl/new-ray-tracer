#include <atomic>

#include "threadBuffer.h"

ThreadBuffer::ThreadBuffer(int startX, int startY, int n) {
    this->startX = startX;
    this->startY = startY;
    this->n = n;
    this->data = new unsigned char[n * 3];
    this->readiness = new std::atomic<bool>[n * 3];
    for(int i = 0; i < (n * 3); i++) {
        this->readiness[i] = false;
    }
}

ThreadBuffer::~ThreadBuffer() {
    delete[] this->data;
}
