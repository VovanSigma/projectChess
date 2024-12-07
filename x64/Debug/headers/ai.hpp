#pragma once

#include <algorithm>
#include <memory>
#include "common.hpp"

namespace ai
{
    struct Board;

    struct Move
    {
        enum class Type
        {
            None,
            Move,
            Fight
        };

        Type type;
        sf::Vector2i start;
        sf::Vector2i end;
    };

    struct Pawn
    {
        ai::Board* board;
        sf::Vector2i position;
        Color color;
        bool isKing;

        void move(const sf::Vector2i& destination, bool duringFight = false);
        bool canMove(const sf::Vector2i& destination) const;
        bool canMove() const;
        std::vector<sf::Vector2i> getMovePositions() const;

        void fight(const sf::Vector2i& destination);
        bool canFight(const sf::Vector2i& destination) const;
        bool canFight() const;
        std::vector<sf::Vector2i> getFightPositions() const;
    };

    struct Board
    {
        ai::Board* parent;
        ai::Move move;
        std::vector<ai::Pawn> pawns;
        std::vector<std::unique_ptr<ai::Board>> children;

        ai::Pawn* getPawn(int x, int y);
        ai::Pawn* getPawn(const sf::Vector2i& position);
        std::vector<ai::Pawn*> getPawns(Color color);
        void killPawn(const sf::Vector2i& position);
        bool isFightPossible(Color color) const;
        bool isMovePossible(Color color) const;
        int pawnCount(Color color) const;
    };

    ai::Move getNextMove(ai::Board* board, Color color, int depth);
    std::pair<ai::Board*, int> minimax(ai::Board* board, Color color, int depth, int alpha, int beta);
    void buildDecisionTree(ai::Board* board, Color color, int depth);
    void ensureFightPriority(ai::Board* board);
    int computeHeuristic(ai::Board* board);
}