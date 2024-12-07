#include "worker.hpp"

Worker::Worker()
: mRunning{ true }
, mStatus{ Status::Waiting }
, mThread{ &Worker::main, this }
, mBoard{}
, mColor{}
, mSearchDepth{}
, mMove{}
{
    mThread.launch();
}

Worker::~Worker()
{
    mRunning = false;
}

void Worker::requestNextMove(Board* board, Color color, int depth)
{
    if (mStatus != Status::Working)
    {
        mBoard = board->getAI();
        mColor = color;
        mSearchDepth = depth;
        mStatus = Status::Working;
    }
}

Worker::Status Worker::getStatus() const
{
    return mStatus;
}

ai::Move Worker::getNextMove()
{
    mStatus = Status::Waiting;
    return mMove;
}

void Worker::main()
{
    while (mRunning)
    {
        if (mStatus != Status::Working) sf::sleep(TimePerFrame);
        else
        {
            mMove = ai::getNextMove(mBoard.get(), mColor, mSearchDepth);
            mBoard.reset();
            mStatus = Status::Finished;
        }
    }
}
