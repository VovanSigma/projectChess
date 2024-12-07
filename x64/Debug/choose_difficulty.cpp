#include "state_stack.hpp"
#include "choose_difficulty.hpp"

ChooseDifficulty::ChooseDifficulty()
: mHeader{ "Difficulty", Resources::get().font("Candara"), 70 }
, mEasy{ "Easy", 224.f }
, mNormal{ "Normal", 224.f + 128.f }
, mHard{ "Hard", 224.f + 2.f * 128.f }
, mBack{ "Back", 224.f + 3.f * 128.f }
, mTransition{ false }
, mTransitionTimer{ sf::Time::Zero }
{
    centerOrigin(mHeader);
    mHeader.setPosition(WindowWidth / 2.f, 96.f);
    mHeader.setOutlineThickness(2.f);

    mEasy.setCallback([this]() 
    {
        StateStack::get().pop();
        StateStack::get().push(State::Type::Game, 1);
    });

    mNormal.setCallback([this]()
    {
        StateStack::get().pop();
        StateStack::get().push(State::Type::Game, 2);
    });

    mHard.setCallback([this]()
    {
        StateStack::get().pop();
        StateStack::get().push(State::Type::Game, 3);
    });

    mBack.setCallback([this]()
    {
        StateStack::get().pop();
    });
}

void ChooseDifficulty::onPush(State::Message message)
{
    mTransition = true;
    mTransitionTimer = sf::Time::Zero;
}

void ChooseDifficulty::onPop(State::Message message)
{
}

void ChooseDifficulty::onFocusGain()
{
    mTransition = true;
    mTransitionTimer = sf::Time::Zero;
}

void ChooseDifficulty::onFocusLoss()
{
}

void ChooseDifficulty::processEvent(const sf::Event& event)
{
    if (mTransition) return;

    switch (event.type)
    {
        case sf::Event::Closed:
        {
            StateStack::get().closeWindow();
        } 
        break;

        case sf::Event::KeyPressed:
        {
            if (event.key.code == sf::Keyboard::Escape) StateStack::get().pop();
        } 
        break;
    }

    mEasy.processEvent(event);
    mNormal.processEvent(event);
    mHard.processEvent(event);
    mBack.processEvent(event);
}

void ChooseDifficulty::update(sf::Time dt)
{
    if (mTransition)
    {
        mTransitionTimer += TimePerFrame;
        if (mTransitionTimer >= sf::seconds(1.0f)) mTransition = false;
    }

    mEasy.update(dt);
    mNormal.update(dt);
    mHard.update(dt);
    mBack.update(dt);
}

void ChooseDifficulty::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mTransition) states.shader = Shaders::transition(mTransitionTimer / sf::seconds(1.0f));

    target.draw(mEasy, states);
    target.draw(mNormal, states);
    target.draw(mHard, states);
    target.draw(mBack, states);

    if (!mTransition) states.shader = Shaders::wave(StateStack::get().globalTimer().asSeconds(), { 1.f, 1.f });

    target.draw(mHeader, states);
}
