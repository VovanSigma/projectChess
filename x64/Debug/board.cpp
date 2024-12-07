#include "board.hpp"

Board::Board()
: mSprite{ Resources::get().texture("Board") }
, mPawns{}
{
    reset();
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
    for (const auto& pawn : mPawns) target.draw(*pawn, states);
}

Pawn* Board::getPawn(int x, int y)
{
    if (x < 0 || x >= BoardWidth) return nullptr;

    if (y < 0 || y >= BoardHeight) return nullptr;

    for (const auto& pawn : mPawns)
        if (pawn->getPosition() == sf::Vector2i{x, y}) return pawn.get();

    return nullptr;
}

Pawn* Board::getPawn(const sf::Vector2i& position)
{
    return getPawn(position.x, position.y);
}

std::vector<Pawn*> Board::getPawns(Color color)
{
    std::vector<Pawn*> pawns;

    for (const auto& pawn : mPawns)
        if (pawn->getColor() == color) pawns.push_back(pawn.get());

    return pawns;
}

void Board::killPawn(const sf::Vector2i& position)
{
    mPawns.erase(
        std::remove_if(
            mPawns .begin(),
            mPawns.end(),
            [&position](const std::unique_ptr<Pawn>& pawn) { return pawn->getPosition() == position; }
        ),
        mPawns.end()
    );
}

bool Board::isFightPossible(Color color) const
{
    for (const auto& pawn : mPawns)
        if (pawn->getColor() == color && pawn->canFight())
            return true;

    return false;
}

bool Board::isMovePossible(Color color) const
{
    for (const auto& pawn : mPawns)
        if (pawn->getColor() == color && pawn->canMove())
            return true;

    return false;
}

int Board::pawnCount(Color color) const
{
    int count = 0;

    for (const auto& pawn : mPawns)
        if (pawn->getColor() == color)
            count++;

    return count;
}

void Board::reset()
{
    mPawns.clear();

    ADD_LIGHT_PAWN(1, 0);
    ADD_LIGHT_PAWN(3, 0);
    ADD_LIGHT_PAWN(5, 0);
    ADD_LIGHT_PAWN(7, 0);
    ADD_LIGHT_PAWN(0, 1);
    ADD_LIGHT_PAWN(2, 1);
    ADD_LIGHT_PAWN(4, 1);
    ADD_LIGHT_PAWN(6, 1);
    ADD_LIGHT_PAWN(1, 2);
    ADD_LIGHT_PAWN(3, 2);
    ADD_LIGHT_PAWN(5, 2);
    ADD_LIGHT_PAWN(7, 2);

    ADD_DARK_PAWN(0, 5);
    ADD_DARK_PAWN(2, 5);
    ADD_DARK_PAWN(4, 5);
    ADD_DARK_PAWN(6, 5);
    ADD_DARK_PAWN(1, 6);
    ADD_DARK_PAWN(3, 6);
    ADD_DARK_PAWN(5, 6);
    ADD_DARK_PAWN(7, 6);
    ADD_DARK_PAWN(0, 7);
    ADD_DARK_PAWN(2, 7);
    ADD_DARK_PAWN(4, 7);
    ADD_DARK_PAWN(6, 7);
}

std::unique_ptr<ai::Board> Board::getAI() const
{
    auto board = std::make_unique<ai::Board>();

    board->parent = nullptr;
    board->move = ai::Move{ ai::Move::Type::None, sf::Vector2i{}, sf::Vector2i{} };

    for (const auto& pawn : mPawns)
    {
        auto p = pawn->getAI();
        p.board = board.get();

        board->pawns.push_back(p);
    }

    return board;
}