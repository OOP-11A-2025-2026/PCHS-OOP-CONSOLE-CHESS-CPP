#include "timer/Timer.h"
#include <iostream>
#include <iomanip>
#include <sstream>

/**
 * Constructs a timer with specified minutes per player.
 */
Timer::Timer(int minutesPerPlayer)
    : whiteTime(minutesPerPlayer * 60),
      blackTime(minutesPerPlayer * 60),
      running(Color::WHITE),
      stopFlag(false),
      totalSeconds(minutesPerPlayer * 60) {}

/**
 * Destructor - stops the timer thread.
 */
Timer::~Timer() {
    stop();
}

/**
 * Starts the timer countdown.
 */
void Timer::start() {
    stopFlag = false;
    timerThread = std::thread(&Timer::timerLoop, this);
}

/**
 * Stops the timer.
 */
void Timer::stop() {
    stopFlag = true;
    if (timerThread.joinable()) {
        timerThread.join();
    }
}

/**
 * Timer loop that runs in background thread.
 */
void Timer::timerLoop() {
    while (!stopFlag) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        if (stopFlag) break;

        if (running == Color::WHITE) {
            whiteTime--;
            if (whiteTime <= 0) {
                std::cout << "\n⏱ WHITE ran out of time! BLACK wins!" << std::endl;
                stopFlag = true;
                // Note: In a real application, you'd want to signal the game to end
                // rather than calling exit() directly
                // For now, we'll just set stopFlag and let the game handle it
                break;
            }
        } else {
            blackTime--;
            if (blackTime <= 0) {
                std::cout << "\n⏱ BLACK ran out of time! WHITE wins!" << std::endl;
                stopFlag = true;
                break;
            }
        }
    }
}

/**
 * Switches the turn to the other player.
 */
void Timer::switchTurn() {
    running = (running == Color::WHITE) ? Color::BLACK : Color::WHITE;
    printTime();
}

/**
 * Prints the current time for both players.
 */
void Timer::printTime() const {
    std::cout << "⏱ White: " << format(whiteTime) 
              << "   Black: " << format(blackTime) << std::endl;
}

/**
 * Formats seconds into MM:SS format.
 */
std::string Timer::format(int seconds) const {
    int m = seconds / 60;
    int s = seconds % 60;
    
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << m 
        << ":" 
        << std::setfill('0') << std::setw(2) << s;
    return oss.str();
}

/**
 * Gets remaining time for a player.
 */
int Timer::getRemainingSeconds(Color color) const {
    return (color == Color::WHITE) ? whiteTime.load() : blackTime.load();
}

/**
 * Checks if a player has run out of time.
 */
bool Timer::isTimeOver(Color color) const {
    return getRemainingSeconds(color) <= 0;
}