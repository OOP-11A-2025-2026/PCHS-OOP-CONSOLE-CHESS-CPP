#include "board/Timer.h"

Timer::Timer(int minutes)
    : totalSeconds(minutes * 60),
      remainingWhite(totalSeconds),
      remainingBlack(totalSeconds),
      running(false),
      currentTurn(Color::WHITE) {}

void Timer::start(Color turn) {
    running = true;
    currentTurn = turn;
    startTime = std::chrono::steady_clock::now();
}

void Timer::stop() {
    if (!running) return;

    auto now = std::chrono::steady_clock::now();
    int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();

    if (currentTurn == Color::WHITE)
        remainingWhite -= elapsed;
    else
        remainingBlack -= elapsed;

    running = false;
}

void Timer::reset() {
    remainingWhite = totalSeconds;
    remainingBlack = totalSeconds;
    running = false;
    currentTurn = Color::WHITE;
}

void Timer::switchTurn(Color newTurn) {
    stop();
    start(newTurn);
}

int Timer::getRemainingSeconds(Color turn) const {
    if (turn == Color::WHITE) return remainingWhite;
    return remainingBlack;
}

bool Timer::isTimeOver(Color turn) const {
    return getRemainingSeconds(turn) <= 0;
}
