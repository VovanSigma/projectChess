#include "button.hpp"

Button::Button(const std::string& text, float height, std::function<void()> callback)
: mSprite{ Resources::get().texture("Button") }
, mText{ text, Resources::get().font("Candara"), 30 }
, mSelectionTimer{ sf::Time::Zero }
, mSoundTimer{ sf::Time::Zero }
, mHover{ false }
, mCallback{ callback }
{
    centerOrigin(mSprite);
    mSprite.setPosition(WindowWidth / 2.f, height);

    centerOrigin(mText);
    mText.setPosition(WindowWidth / 2.f, height);
    mText.setOutlineThickness(1.f);
}

void Button::processEvent(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::MouseMoved:
        {
            auto mouseX = static_cast<float>(event.mouseMove.x);
            auto mouseY = static_cast<float>(event.mouseMove.y);

            if (isInside({mouseX, mouseY}))
            {
                mHover = true;

                if (mSoundTimer == sf::Time::Zero) 
                {
                    SoundPlayer::get().play("Hover", 25, 1.0f);
                    mSoundTimer = TimePerFrame;
                }
            }
            else
            {
                mHover = false;
                mSelectionTimer = sf::Time::Zero;
            }
        } 
        break;

        case sf::Event::MouseButtonReleased:
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                auto mouseX = static_cast<float>(event.mouseButton.x);
                auto mouseY = static_cast<float>(event.mouseButton.y);

                if (isInside({ mouseX, mouseY }))
                {
                    mCallback();
                    mHover = false;
                    mSelectionTimer = sf::Time::Zero;
                    SoundPlayer::get().play("Click", 100, 1.0f);
                }
            }
        } 
        break;
    }
}

void Button::update(sf::Time dt)
{
    if (mHover) mSelectionTimer += dt;
    else
    {
        mSoundTimer -= dt;
        if (mSoundTimer < sf::Time::Zero) mSoundTimer = sf::Time::Zero;
    }

}

void Button::setCallback(std::function<void()> callback)
{
    mCallback = callback;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mHover) states.shader = Shaders::selection(mSelectionTimer.asSeconds());

    target.draw(mSprite, states);
    target.draw(mText, states);
}

bool Button::isInside(const sf::Vector2f& position)
{
    return std::abs(mSprite.getPosition().x - position.x) <= ButtonWidth / 2.f 
        && std::abs(mSprite.getPosition().y - position.y) <= ButtonHeight / 2.f;
}