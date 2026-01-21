#ifndef TIMER_H
#define TIMER_H

#include "enums/Color.h"
#include <chrono>
#include <thread>
#include <atomic>
#include <string>

/**
 * Game timer that tracks time for both players.
 * Runs in a background thread and decrements time every second.
 */
class Timer {
private:
    std::atomic<int> whiteTime;
    std::atomic<int> blackTime;
    std::atomic<Color> running;
    std::thread timerThread;
    std::atomic<bool> stopFlag;
    int totalSeconds;

    /**
     * Timer loop that runs in background thread.
     */
    void timerLoop();

    /**
     * Formats seconds into MM:SS format.
     * @param seconds The time in seconds
     * @return Formatted string (e.g., "05:30")
     */
    std::string format(int seconds) const;

public:
    /**
     * Constructs a timer with specified minutes per player.
     * @param minutesPerPlayer Starting time for each player in minutes
     */
    Timer(int minutesPerPlayer);

    /**
     * Destructor - stops the timer thread.
     */
    ~Timer();

    /**
     * Starts the timer countdown.
     */
    void start();

    /**
     * Stops the timer.
     */
    void stop();

    /**
     * Switches the turn to the other player.
     */
    void switchTurn();

    /**
     * Prints the current time for both players.
     */
    void printTime() const;

    /**
     * Gets remaining time for a player.
     * @param color The player's color
     * @return Remaining seconds
     */
    int getRemainingSeconds(Color color) const;

    /**
     * Checks if a player has run out of time.
     * @param color The player's color
     * @return true if time is up
     */
    bool isTimeOver(Color color) const;
};

#endif // TIMER_H