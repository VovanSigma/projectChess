#include "slider.hpp"

Slider::Slider(int minValue, int maxValue, int currentValue, float height, std::function<void(int)> callback)
: mMinValue{ minValue }
, mUnitLength{ SliderWidth / static_cast<float>(maxValue - minValue) }
, mBar{ Resources::get().texture("Bar") }
, mArrow{ Resources::get().texture("Arrow") }
, mMoving{ false }
, mCallback{ callback }
{
    centerOrigin(mBar);
    mBar.setPosition(WindowWidth / 2.f, height);

    centerOrigin(mArrow);
    mArrow.setPosition(WindowWidth / 2.f, height);

    moveArrow(static_cast<float>(currentValue - minValue) * mUnitLength);
}

void Slider::processEvent(const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::MouseButtonPressed:
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                auto mouseX = static_cast<float>(event.mouseButton.x);
                auto mouseY = static_cast<float>(event.mouseButton.y);

                if (std::abs(mouseX - mBar.getPosition().x) <= SliderWidth / 2.f && 
                    std::abs(mouseY - mBar.getPosition().y) <= SliderHeight / 2.f)
                {
                    moveArrow(mouseX - (mBar.getPosition().x - SliderWidth / 2.f));
                    mMoving = true;
                }
            }
        } 
        break;

        case sf::Event::MouseButtonReleased:
        {
            if (event.mouseButton.button == sf::Mouse::Left) mMoving = false;
        } 
        break;

        case sf::Event::MouseMoved:
        {
            if (mMoving)
            {
                auto x = static_cast<float>(event.mouseMove.x);
                auto y = static_cast<float>(event.mouseMove.y);

                auto dx = x - (mBar.getPosition().x - SliderWidth / 2.f);

                if (x - mBar.getPosition().x >= SliderWidth / 2.f) dx = SliderWidth;
                else if (x - mBar.getPosition().x <= -SliderWidth / 2.f) dx = 0.f;

                moveArrow(dx);
            }
        } 
        break;
    }
}

void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mBar, states);
    target.draw(mArrow, states);
}

void Slider::moveArrow(float dx)
{
    int units = static_cast<int>(dx / mUnitLength);
    int value = mMinValue + units;

    mArrow.setPosition(mBar.getPosition().x - SliderWidth / 2.f + dx, mArrow.getPosition().y);
    mCallback(value);
}
