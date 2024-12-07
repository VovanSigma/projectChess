#include "shaders.hpp"

sf::Shader* Shaders::selection(float time)
{
    auto* selection = Resources::get().shader("Selection");
    selection->setUniform("time", time);
    return selection;
}

sf::Shader* Shaders::shadow(float time)
{
    auto* shadow = Resources::get().shader("Shadow");
    shadow->setUniform("time", time);
    return shadow;
}

sf::Shader* Shaders::transition(float progress)
{
    auto* transition = Resources::get().shader("Transition");
    transition->setUniform("progress", progress);
    return transition;
}

sf::Shader* Shaders::wave(float phase, sf::Vector2f amplitude)
{
    auto* wave = Resources::get().shader("Wave");
    wave->setUniform("phase", phase);
    wave->setUniform("amplitude", amplitude);
    return wave;
}