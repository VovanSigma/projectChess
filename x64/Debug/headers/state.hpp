#pragma once

#include <memory>
#include <variant>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class State : public sf::Drawable
{
public:
    using Ptr = std::unique_ptr<State>;
    using Message = std::variant<int>;

    enum class Type
    {
        Game,
        Options,
        Title,
        ChooseDifficulty
    };

    virtual ~State() {};

    virtual void onPush(State::Message message) = 0;
    virtual void onPop(State::Message message) = 0;
    virtual void onFocusGain() = 0;
    virtual void onFocusLoss() = 0;

    virtual void processEvent(const sf::Event& event) = 0;
    virtual void update(sf::Time dt) = 0;
};