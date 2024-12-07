#pragma once

#include <functional>
#include "common.hpp"
#include "resources.hpp"
#include "shaders.hpp"
#include "sound_player.hpp"

class Button : public sf::Drawable
{
public:
    Button(const std::string& text, float height, std::function<void()> callback = {});
    void processEvent(const sf::Event& event);
    void update(sf::Time dt);

    void setCallback(std::function<void()> callback);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    bool isInside(const sf::Vector2f& position);

private:
    sf::Sprite mSprite;
    sf::Text mText;
    sf::Time mSelectionTimer;
    sf::Time mSoundTimer;
    bool mHover;
    std::function<void()> mCallback;
};