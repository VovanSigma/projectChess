#pragma once

#include "button.hpp"
#include "state.hpp"

class ChooseDifficulty : public State
{
public:
    ChooseDifficulty();

    virtual void onPush(State::Message message) override;
    virtual void onPop(State::Message message) override;
    virtual void onFocusGain() override;
    virtual void onFocusLoss() override;

    virtual void processEvent(const sf::Event& event) override;
    virtual void update(sf::Time dt) override;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Text mHeader;
    Button mEasy;
    Button mNormal;
    Button mHard;
    Button mBack;

    bool mTransition;
    sf::Time mTransitionTimer;
};
