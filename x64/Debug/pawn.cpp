#include "board.hpp"
#include "pawn.hpp"
#include "state_stack.hpp"

Pawn::Pawn(Board* board, const sf::Vector2i& position, Color color)
: mBoard{ board }
, mSprite{ Resources::get().texture(color == Color::Light ? "LightPawn" : "DarkPawn") }
, mPosition{ position }
, mColor{ color }
, mIsKing{ false }
, mIsSelected{ false }
{
    mSprite.setPosition(float(TileSize) * (position.x + OffsetX), float(TileSize) * (position.y + OffsetY));
}

void Pawn::select(bool value)
{
    std::string textureName = "";

    if (mColor == Color::Light) textureName += "Light";
    else textureName += "Dark";

    if (mIsKing) textureName += "King";
    else textureName += "Pawn";

    mSprite.setTexture(Resources::get().texture(textureName));
    mIsSelected = value;
}

void Pawn::move(const sf::Vector2i& destination, bool duringFight)
{
    if (!isValidPosition(destination)) return;

    if (destination == mPosition) return;

    if (mBoard->getPawn(destination) != nullptr) return;

    mPosition = destination;
    mSprite.setPosition(float(TileSize) * (destination.x + OffsetX), float(TileSize) * (destination.y + OffsetY));

    if (!mIsKing && (mColor == Color::Light && destination.y == BoardHeight - 1 || mColor == Color::Dark && destination.y == 0))
    {
        if (duringFight && canFight()) return;

        mIsKing = true;
        mSprite.setTexture(Resources::get().texture(mColor == Color::Light ? "LightKing" : "DarkKing"));
    }

    SoundPlayer::get().play("Move", 100, 1.0f);
}

bool Pawn::canMove(const sf::Vector2i& destination) const
{
    if (!isValidPosition(destination)) return false;

    if (destination == mPosition) return false;

    if (mIsKing)
    {
        // check whether we are moving across some diagonal
        if (abs(destination.x - mPosition.x) != abs(destination.y - mPosition.y)) return false;

        int dx = (destination.x > mPosition.x) ? 1 : -1;
        int dy = (destination.y > mPosition.y) ? 1 : -1;

        bool foundSomebody = false;

        int actualX = mPosition.x + dx;
        int actualY = mPosition.y + dy;

        while (actualX != destination.x && actualY != destination.y)
        {
            if (mBoard->getPawn(actualX, actualY))
            {
                foundSomebody = true;
                break;
            }

            actualX += dx; 
            actualY += dy;
        }

        return !foundSomebody && !mBoard->getPawn(destination);
    }
    else
    {
        int offset = (mColor == Color::Light ? 1 : -1);
        if ((destination.x == mPosition.x - 1 || destination.x == mPosition.x + 1) && (destination.y == mPosition.y + offset))
            return mBoard->getPawn(destination) == nullptr;
    }

    return false;
}

bool Pawn::canMove() const
{
    if (mIsKing)
    {
        for (int i = -1; i <= 1; i += 2)
        {
            for (int j = -1; j <= 1; j += 2)
            {
                int x = mPosition.x + i;
                int y = mPosition.y + j;

                while (isValidPosition(x, y))
                {
                    if (canMove({ x, y })) return true;
                    x += i;
                    y += j;
                }
            }
        }
    }
    else
    {
        int offset = (mColor == Color::Light ? 1 : -1);
        return canMove({ mPosition.x - 1, mPosition.y + offset }) || canMove({ mPosition.x + 1, mPosition.y + offset });
    }

    return false;
}

std::vector<sf::Vector2i> Pawn::getMovePositions() const
{
    std::vector<sf::Vector2i> movePositions{};

    if (mIsKing)
    {
        for (int i = -1; i <= 1; i += 2)
        {
            for (int j = -1; j <= 1; j += 2)
            {
                int x = mPosition.x + i;
                int y = mPosition.y + j;

                while (isValidPosition(x, y))
                {
                    if (canMove({ x, y })) movePositions.push_back({ x, y });
                    x += i;
                    y += j;
                }
            }
        }
    }
    else
    {
        int offset = (mColor == Color::Light ? 1 : -1);

        if (canMove({ mPosition.x - 1, mPosition.y + offset }))
            movePositions.push_back({ mPosition.x - 1, mPosition.y + offset });

        if (canMove({ mPosition.x + 1, mPosition.y + offset }))
            movePositions.push_back({ mPosition.x + 1, mPosition.y + offset });
    }

    return movePositions;
}

void Pawn::fight(const sf::Vector2i& destination)
{
    if (!isValidPosition(destination)) return;

    if (destination == mPosition) return;

    if (mBoard->getPawn(destination) != nullptr) return;

    if (mIsKing)
    {
        int dx = (destination.x > mPosition.x) ? 1 : -1;
        int dy = (destination.y > mPosition.y) ? 1 : -1;

        int actualX = mPosition.x + dx;
        int actualY = mPosition.y + dy;

        while (actualX != destination.x && actualY != destination.y)
        {
            auto* pawn = mBoard->getPawn({ actualX, actualY });
            if (pawn && pawn->getColor() != mColor)
            {
                mBoard->killPawn(pawn->getPosition());
                move(destination, true);
                break;
            }

            actualX += dx;
            actualY += dy;
        }
    }
    else
    {
        Pawn* pawn = nullptr;

        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (destination.x == mPosition.x + 2 * i && destination.y == mPosition.y + 2 * j)
                {
                    pawn = mBoard->getPawn(mPosition.x + i, mPosition.y + j);
                    break;
                }
            }
        }

        if (pawn && pawn->getColor() != mColor)
        {
            mBoard->killPawn(pawn->getPosition());
            move(destination, true);
        }
    }

    SoundPlayer::get().play("Fight", 100, 1.0f);
}

bool Pawn::canFight(const sf::Vector2i& destination) const
{
    if (!isValidPosition(destination)) return false;

    if (destination == mPosition) return false;

    if (mBoard->getPawn(destination) != nullptr) return false;

    if (mIsKing)
    {
        // check whether we are moving across some diagonal
        if (abs(destination.x - mPosition.x) != abs(destination.y - mPosition.y)) return false;

        int dx = (destination.x > mPosition.x) ? 1 : -1;
        int dy = (destination.y > mPosition.y) ? 1 : -1;

        int foundEnemies = 0;
        int foundAllies = 0;

        int actualX = mPosition.x + dx;
        int actualY = mPosition.y + dy;

        while (actualX != destination.x && actualY != destination.y)
        {
            auto* pawn = mBoard->getPawn(actualX, actualY);
            if (pawn)
            {
                if (pawn->getColor() == mColor)
                    foundAllies++;
                else
                    foundEnemies++;
            }

            actualX += dx;
            actualY += dy;
        }

        return foundAllies == 0 && foundEnemies == 1 && !mBoard->getPawn(destination);
    }

    Pawn* pawn = nullptr;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (destination.x == mPosition.x + 2 * i && destination.y == mPosition.y + 2 * j)
            {
                pawn = mBoard->getPawn(mPosition.x + i, mPosition.y + j);
                break;
            }
        }
    }

    return pawn && pawn->getColor() != mColor;
}

bool Pawn::canFight() const
{
    if (mIsKing)
    {
        for (int i = -1; i <= 1; i += 2)
        {
            for (int j = -1; j <= 1; j += 2)
            {
                int x = mPosition.x + i;
                int y = mPosition.y + j;

                while (isValidPosition(x, y))
                {
                    if (canFight({ x, y })) return true;
                    x += i;
                    y += j;
                }
            }
        }

        return false;
    }

    return canFight({ mPosition.x + 2, mPosition.y + 2 })
        || canFight({ mPosition.x - 2, mPosition.y + 2 })
        || canFight({ mPosition.x + 2, mPosition.y - 2 })
        || canFight({ mPosition.x - 2, mPosition.y - 2 });
}

std::vector<sf::Vector2i> Pawn::getFightPositions() const
{
    std::vector<sf::Vector2i> fightPositions{};

    if (mIsKing)
    {
        for (int i = -1; i <= 1; i += 2)
        {
            for (int j = -1; j <= 1; j += 2)
            {
                int x = mPosition.x + i;
                int y = mPosition.y + j;

                while (isValidPosition(x, y))
                {
                    if (canFight({ x, y })) fightPositions.push_back({ x, y });
                    x += i;
                    y += j;
                }
            }
        }
    }
    else
    {
        for (int i = -2; i <= 2; i += 4)
        {
            for (int j = -2; j <= 2; j += 4)
            {
                if (canFight({ mPosition.x + i, mPosition.y + j }))
                    fightPositions.push_back({ mPosition.x + i, mPosition.y + j });
            }
        }
    }

    return fightPositions;
}

ai::Pawn Pawn::getAI() const
{
    return ai::Pawn{ nullptr, mPosition, mColor, mIsKing };
}

const sf::Vector2i& Pawn::getPosition() const
{
    return mPosition;
}

Color Pawn::getColor() const
{
    return mColor;
}

bool Pawn::isKing() const
{
    return mIsKing;
}

bool Pawn::isSelected() const
{
    return mIsSelected;
}

void Pawn::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mIsSelected) states.shader = Shaders::selection(StateStack::get().globalTimer().asSeconds());
    target.draw(mSprite, states);
}