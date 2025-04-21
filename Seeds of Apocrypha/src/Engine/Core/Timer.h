#pragma once
#include <chrono>

class Timer {
public:
    using clock = std::chrono::high_resolution_clock;
    using time_point = std::chrono::time_point<clock>;

    Timer() {}

    void Start() {
        start_time = clock::now();
        running = true;
        paused = false;
        elapsed = std::chrono::duration<double>(0);
    }

    void Stop() {
        if (running && !paused) {
            elapsed += clock::now() - start_time;
        }
        running = false;
        paused = false;
    }

    void Pause() {
        if (running && !paused) {
            pause_time = clock::now();
            elapsed += pause_time - start_time;
            paused = true;
        }
    }

    void Resume() {
        if (running && paused) {
            start_time = clock::now();
            paused = false;
        }
    }

    void Reset() {
        elapsed = std::chrono::duration<double>(0);
        running = false;
        paused = false;
    }

    double Elapsed() const {
        if (running && !paused) {
            return (elapsed + (clock::now() - start_time)).count();
        }
        return elapsed.count();
    }

    bool IsRunning() const { return running; }
    bool IsPaused() const { return paused; }

private:
    bool running = false;
    bool paused = false;
    time_point start_time;
    time_point pause_time;
    std::chrono::duration<double> elapsed = std::chrono::duration<double>(0);
};