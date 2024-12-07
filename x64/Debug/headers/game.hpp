#pragma once

#include "board.hpp"
#include "pawn.hpp"
#include "sound_player.hpp"
#include "state.hpp"
#include "worker.hpp"

class Game : public State
{
public:
    Game();

    void handlePlayerAction(const sf::Vector2i& destination);
    void handleEnemyAction();
    void nextTurn();
    void tryToFinish();
    void reset();

    virtual void onPush(State::Message message) override;
    virtual void onPop(State::Message message) override;
    virtual void onFocusGain() override;
    virtual void onFocusLoss() override;

    virtual void processEvent(const sf::Event& event) override;
    virtual void update(sf::Time dt) override;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    Board mBoard;
    Pawn* mSelected;
    bool mLock;
    bool mTraining;
    Color mPlayerColor;
    Color mActualPlayerColor;
    int mSearchDepth;
    sf::Time mEnemyTimer;
    sf::Text mTurnText;
    bool mFinished;
    Worker mWorker;
};