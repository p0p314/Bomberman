#include "Debouncer.hpp"

Debouncer::Debouncer(float delay) : delay(delay), elapsedTime(0.0f), ready(true) {}

bool Debouncer::update(float deltaTime) {
    if (!ready) {
        elapsedTime += deltaTime;
        if (elapsedTime >= delay) {
            ready = true;
        }
    }
    return ready;
}

void Debouncer::trigger() {
    if (ready) {
        ready = false;
        elapsedTime = 0.0f;
    }
}

bool Debouncer::isReady() {
    return ready;
}