#include <iostream>
#include "state_stack.hpp"

int main(const int argc, char** argv)
{
    if (argc == 3)
    {
        if (argv[1] == std::string{ "--benchmark" })
        {
            try
            {
                int searchDepth = std::stoi(argv[2]);

                if (searchDepth < 0 || searchDepth > 15)
                    std::cerr << "Invalid value: " << searchDepth << '\n';
                else
                {
                    Board board{};
                    auto boardAI = board.getAI();

                    sf::Clock clock;
                    auto move = ai::getNextMove(boardAI.get(), Color::Light, searchDepth);

                    auto elapsed = clock.getElapsedTime().asMilliseconds();
                    std::cout << "Searching for move with depth " << searchDepth << " took: " << elapsed << " ms" << '\n';
                }
            }
            catch (const std::exception& ex)
            {
                std::cerr << "Exception: " << ex.what() << '\n';
            }
        }
        else
            std::cerr << "Unrecognized command line argument" << '\n';
    }
    else
    {
        try
        {
            StateStack::get().push(State::Type::Title);
            StateStack::get().run();
        }
        catch (const std::exception& ex)
        {
            std::cerr << "Exception: " << ex.what() << '\n';
        }
    }
}