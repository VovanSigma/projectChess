#pragma once

#include "ai.hpp"
#include "resources.hpp"

class Board;

class Pawn : public sf::Drawable
{
public:
    Pawn(Board* board, const sf::Vector2i& position, Color color);

    void select(bool value);
    void move(const sf::Vector2i& destination, bool duringFight = false);
    bool canMove(const sf::Vector2i& destination) const;
    bool canMove() const;
    std::vector<sf::Vector2i> getMovePositions() const;

    void fight(const sf::Vector2i& destination);
    bool canFight(const sf::Vector2i& destination) const;
    bool canFight() const;
    std::vector<sf::Vector2i> getFightPositions() const;

    ai::Pawn getAI() const;

    const sf::Vector2i& getPosition() const;
    Color getColor() const;
    bool isKing() const;
    bool isSelected() const;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    Board* mBoard;
    sf::Sprite mSprite;
    sf::Vector2i mPosition;
    Color mColor;
    bool mIsKing;
    bool mIsSelected;
};