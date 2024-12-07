#pragma once

#include <atomic>
#include <SFML/System/Thread.hpp>
#include "ai.hpp"
#include "board.hpp"

class Worker
{
public:
    enum class Status
    {
        Waiting,
        Working,
        Finished
    };

    Worker();
    ~Worker();
    Worker(const Worker&) = delete;
    Worker& operator=(const Worker&) = delete;

    void requestNextMove(Board* board, Color color, int searchDepth);
    Worker::Status getStatus() const;
    ai::Move getNextMove();

    void main();

private:
    std::atomic<bool> mRunning;
    std::atomic<Worker::Status> mStatus;
    sf::Thread mThread;

    std::unique_ptr<ai::Board> mBoard;
    Color mColor;
    int mSearchDepth;
    ai::Move mMove;
};
