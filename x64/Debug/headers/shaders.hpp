#pragma once

#include "common.hpp"
#include "resources.hpp"

class Shaders
{
public:
    static sf::Shader* selection(float time);
    static sf::Shader* shadow(float time);
    static sf::Shader* transition(float progress);
    static sf::Shader* wave(float phase, sf::Vector2f amplitude);
};