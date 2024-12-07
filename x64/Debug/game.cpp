#include "game.hpp"
#include "state_stack.hpp"

Game::Game()
: mBoard{}
, mSelected{ nullptr }
, mLock{ false }
, mTraining{}
, mPlayerColor{ Color::Light }
, mActualPlayerColor{ Color::Light }
, mSearchDepth{}
, mEnemyTimer{}
, mTurnText{ "White Player Turn", Resources::get().font("Candara"), 30 }
, mFinished{ false }
, mWorker{}
{
    centerOrigin(mTurnText);
    mTurnText.setPosition(WindowWidth / 2.f, 96.f);
}

void Game::handlePlayerAction(const sf::Vector2i& destination)
{
    bool fightPossible = mBoard.isFightPossible(mActualPlayerColor);

    if (!mSelected)
    {
        mSelected = mBoard.getPawn(destination);
        if (mSelected && mSelected->getColor() == mActualPlayerColor && (!fightPossible || (fightPossible && mSelected->canFight())))
        {
            mSelected->select(true);
            SoundPlayer::get().play("Success", 100, 1.0f);
        }
        else
        {
            mSelected = nullptr;
            SoundPlayer::get().play("Failure", 100, 1.0f);
        }
    }
    else
    {
        bool canFight = mSelected->canFight();

        if (canFight && mSelected->canFight(destination))
        {
            mSelected->fight(destination);

            if (!mSelected->canFight())
            {
                mSelected->select(false);
                mSelected = nullptr;
                nextTurn();
                mLock = false;
            }
            else
            {
                mLock = true;
            }

        }
        else if (!canFight && mSelected->canMove(destination))
        {
            mSelected->move(destination);
            mSelected->select(false);
            mSelected = nullptr;
            nextTurn();
        }
        else if(!mLock)
        {
            mSelected->select(false);
            mSelected = nullptr;
            SoundPlayer::get().play("Failure", 100.f, 1.0f);
        }
        else
        {
            SoundPlayer::get().play("Failure", 100, 1.0f);
        }
    }

    tryToFinish();
}

void Game::handleEnemyAction()
{
    if (mSelected)
    {
        if (mSelected->canFight())
        {
            if (mEnemyTimer >= sf::seconds(1.f))
                mSelected->fight(mSelected->getFightPositions()[0]);
        }
        else
        {
            mSelected = nullptr;
            nextTurn();
            tryToFinish();

            mEnemyTimer = sf::Time::Zero;
        }
    }
    else
    {
        auto status = mWorker.getStatus();

        if (status == Worker::Status::Waiting)
            mWorker.requestNextMove(&mBoard, mActualPlayerColor, mSearchDepth);
        else if (status == Worker::Status::Finished && mEnemyTimer >= sf::seconds(1.f))
        {
            auto move = mWorker.getNextMove();

            assert(move.type != ai::Move::Type::None);

            mSelected = mBoard.getPawn(move.start);

            assert(mSelected != nullptr);

            if (move.type == ai::Move::Type::Move)
            {
                mSelected->move(move.end);

                mSelected = nullptr;
                nextTurn();
                tryToFinish();

                mEnemyTimer = sf::Time::Zero;
            }
            else
            {
                mSelected->fight(move.end);
                mEnemyTimer = sf::Time::Zero;
            }
        }
    }
}

void Game::nextTurn()
{
    if (mActualPlayerColor == Color::Light)
    {
        mActualPlayerColor = Color::Dark;
        mTurnText.setString("Black Player Turn");
    }
    else
    {
        mActualPlayerColor = Color::Light;
        mTurnText.setString("White Player Turn");
    }
   
    centerOrigin(mTurnText);
}

void Game::tryToFinish()
{
    if ((!mBoard.isFightPossible(mActualPlayerColor) && !mBoard.isMovePossible(mActualPlayerColor)) || mBoard.pawnCount(mActualPlayerColor) == 0)
    {
        mFinished = true;
        mTurnText.setString(mActualPlayerColor == Color::Light ? "Black Player Won!" : "White Player Won!");
        centerOrigin(mTurnText);

        if (!mTraining)
        {
            SoundPlayer::get().playMusic(false);

            if(mActualPlayerColor != mPlayerColor) SoundPlayer::get().play("Victory", 100, 1.0f);
            else SoundPlayer::get().play("GameOver", 100, 1.0f);
        }
    }
}

void Game::reset()
{
    mBoard.reset();
    mSelected = nullptr;
    mLock = false;
    mTraining = false;
    mPlayerColor = Color::Light;
    mActualPlayerColor = Color::Light;
    mSearchDepth = 0;
    mEnemyTimer = sf::Time::Zero;
    
    mTurnText.setString("White Player Turn");
    centerOrigin(mTurnText);

    mFinished = false;

    SoundPlayer::get().playMusic(true);
}

void Game::onPush(State::Message message)
{
    switch (std::get<int>(message))
    {
        case 0:
        {
            mTraining = true;
        } break;

        case 1:
        {
            mTraining = false;
            mSearchDepth = 1;
        } break;

        case 2:
        {
            mTraining = false;
            mSearchDepth = 2;
        } break;

        case 3:
        {
            mTraining = false;
            mSearchDepth = 4;

        } break;
    }
}

void Game::onPop(State::Message message)
{
    reset();
}

void Game::onFocusGain()
{

}

void Game::onFocusLoss()
{
    if (mSelected)
    {
        mSelected->select(false);
        mSelected = nullptr;
    }
}

void Game::processEvent(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::Closed:
        {
            StateStack::get().closeWindow();
        } break;

        case sf::Event::KeyPressed:
        {
            if (mFinished || (event.key.code == sf::Keyboard::F4 && event.key.alt))
                StateStack::get().pop();
            else if (event.key.code == sf::Keyboard::Escape)
                StateStack::get().push(State::Type::Options);
            else if (event.key.code == sf::Keyboard::Q)
                StateStack::get().pop();
        } break;

        case sf::Event::MouseButtonPressed:
        {
            if (mFinished)
                StateStack::get().pop();
            else if (!mFinished && event.mouseButton.button == sf::Mouse::Left)
            {
                if (mTraining || (!mTraining && mActualPlayerColor == mPlayerColor))
                    handlePlayerAction({ event.mouseButton.x / TileSize - OffsetX, event.mouseButton.y / TileSize - OffsetY });
            }
        } break;
    }
}

void Game::update(sf::Time dt)
{
    if (!mFinished && !mTraining && mActualPlayerColor != mPlayerColor)
    {
        mEnemyTimer += dt;
        handleEnemyAction();
    }
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mBoard, states);
    target.draw(mTurnText, states);
}