#include "options.hpp"
#include "state_stack.hpp"

Options::Options()
: mHeader{ "Options", Resources::get().font("Candara"), 70 }
, mNextSong{ "Next Song", 224.f }
, mMusicText{ "Music Volume", Resources::get().font("Candara"), 30 }
, mMusicVolume{ 0, 100, 10, 224.f + 128.f, [](int value) {
    SoundPlayer::get().setMusicVolume(static_cast<float>(value));
} }
, mSoundsText{ "Sounds Volume", Resources::get().font("Candara"), 30 }
, mSoundsVolume{ 0, 100, 40, 224.f + 256.f, [](int value) {
    SoundPlayer::get().setSoundsVolume(static_cast<float>(value));
} }
, mBack{ "Back", 224.f + 384.f }
, mTransition{}
, mTransitionTimer{}
{
    centerOrigin(mHeader);
    mHeader.setPosition(WindowWidth / 2.f, 96.f);
    mHeader.setOutlineThickness(2.f);

    centerOrigin(mMusicText);
    mMusicText.setPosition(WindowWidth / 2.f, 224.f + 96.f);
    mMusicText.setOutlineThickness(1.f);

    centerOrigin(mSoundsText);
    mSoundsText.setPosition(WindowWidth / 2.f, 224.f + 224.f);
    mSoundsText.setOutlineThickness(1.f);

    mNextSong.setCallback([]() 
    {
        SoundPlayer::get().playMusic(false);
        SoundPlayer::get().updateMusic();
        SoundPlayer::get().playMusic(true);
    });

    mBack.setCallback([]() 
    {
        StateStack::get().pop();
    });
}

void Options::onPush(State::Message message)
{
    mTransition = true;
    mTransitionTimer = sf::Time::Zero;
}

void Options::onPop(State::Message message)
{

}

void Options::onFocusGain()
{
}

void Options::onFocusLoss()
{

}

void Options::processEvent(const sf::Event& event)
{
    if (mTransition) return;

    switch (event.type)
    {
        case sf::Event::Closed:
        {
            StateStack::get().closeWindow();
        } break;

        case sf::Event::KeyPressed:
        {
            if (event.key.code == sf::Keyboard::Escape) StateStack::get().pop();
        } break;
    }

    mNextSong.processEvent(event);
    mMusicVolume.processEvent(event);
    mSoundsVolume.processEvent(event);
    mBack.processEvent(event);
}

void Options::update(sf::Time dt)
{
    if (mTransition)
    {
        mTransitionTimer += TimePerFrame;
        if (mTransitionTimer >= sf::seconds(1.0f)) mTransition = false;
    }

    mNextSong.update(dt);
    mBack.update(dt);
}

void Options::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mTransition) states.shader = Shaders::transition(mTransitionTimer / sf::seconds(1.0f));

    target.draw(mNextSong, states);
    target.draw(mMusicText, states);
    target.draw(mMusicVolume, states);
    target.draw(mSoundsText, states);
    target.draw(mSoundsVolume, states);
    target.draw(mBack, states);

    if (!mTransition) states.shader = Shaders::wave(StateStack::get().globalTimer().asSeconds(), { 1.f, 1.f });

    target.draw(mHeader, states);
}