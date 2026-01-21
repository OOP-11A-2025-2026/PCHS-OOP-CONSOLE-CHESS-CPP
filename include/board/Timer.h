#pragma once

#include "enums/Color.h" 
#include <chrono>

class Timer {
private:
    int totalSeconds;
    int remainingWhite;
    int remainingBlack;

    std::chrono::steady_clock::time_point startTime;
    bool running;

    Color currentTurn;

public:
    Timer(int minutes);

    void start(Color turn);
    void stop();
    void reset();

    void switchTurn(Color newTurn);

    int getRemainingSeconds(Color turn) const;
    bool isTimeOver(Color turn) const;
};
