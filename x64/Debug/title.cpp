#include "state_stack.hpp"
#include "title.hpp"

Title::Title()
: mHeader{ "Checkers", Resources::get().font("Candara"), 70 }
, mNewGame{ "New Game", 224.f }
, mOptions{ "Options", 224.f + 128.f }
, mExit{ "Exit", 224.f + 2.f * 128.f }
, mVersion{ "v1.0", Resources::get().font("Candara"), 30 }
, mTransition{ false }
, mTransitionTimer{ sf::Time::Zero }
{
    centerOrigin(mHeader);
    mHeader.setPosition(WindowWidth / 2.f, 96.f);
    mHeader.setOutlineThickness(2.f);

    centerOrigin(mVersion);
    mVersion.setPosition(WindowWidth / 2.f, 224.f + 512.f);

    mNewGame.setCallback([this]()
    {
        StateStack::get().push(State::Type::ChooseDifficulty);
    });

    mOptions.setCallback([this]()
    {
        StateStack::get().push(State::Type::Options);
    });

    mExit.setCallback([this]()
    {
        StateStack::get().closeWindow();
    });
}

void Title::transition()
{
    mTransition = true;
    mTransitionTimer = sf::Time::Zero;
}

void Title::onPush(State::Message message)
{
    transition();
}

void Title::onPop(State::Message message)
{
}

void Title::onFocusGain()
{
    transition();
}

void Title::onFocusLoss()
{
}

void Title::processEvent(const sf::Event& event)
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
            if (event.key.code == sf::Keyboard::Escape) StateStack::get().closeWindow();
        } 
        break;
    }

    mNewGame.processEvent(event);
    mOptions.processEvent(event);
    mExit.processEvent(event);
}

void Title::update(sf::Time dt)
{
    if (mTransition)
    {
        mTransitionTimer += TimePerFrame;
        if (mTransitionTimer >= sf::seconds(1.0f)) mTransition = false;
    }

    mNewGame.update(dt);
    mOptions.update(dt);
    mExit.update(dt);
}

void Title::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mTransition) states.shader = Shaders::transition(mTransitionTimer / sf::seconds(1.0f));

    target.draw(mNewGame, states);
    target.draw(mOptions, states);
    target.draw(mExit, states);
    target.draw(mVersion, states);

    if (!mTransition) states.shader = Shaders::wave(StateStack::get().globalTimer().asSeconds(), { 1.f, 1.f });

    target.draw(mHeader, states);
}