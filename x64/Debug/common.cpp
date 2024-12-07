#include "common.hpp"

bool isValidPosition(int x, int y)
{
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

bool isValidPosition(const sf::Vector2i& position)
{
    return position.x >= 0 && position.x < 8 && position.y >= 0 && position.y < 8;
}

void centerOrigin(sf::Text& text)
{
    sf::FloatRect rect = text.getLocalBounds();
    text.setOrigin(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f);
}

void centerOrigin(sf::Sprite& sprite)
{
    sf::FloatRect rect = sprite.getLocalBounds();
    sprite.setOrigin(rect.width / 2.f, rect.height / 2.f);
}

size_t randomInt(size_t min, size_t max)
{
    std::uniform_int_distribution<size_t> dist(min, max);
    return dist(engine);
}

std::vector<std::pair<std::string, std::string>> getDirectoryEntries(const std::string& directoryPath, std::vector<std::string> extensions)
{
    if (!fs::exists(directoryPath))
        throw std::runtime_error("Unable to find anything at '" + directoryPath + "'");

    if (!fs::is_directory(directoryPath))
        throw std::runtime_error("Unable to find directory at '" + directoryPath + "'");

    std::vector<std::pair<std::string, std::string>> entries;

    for (const auto& entry : fs::directory_iterator(directoryPath))
    {
        const auto& path = entry.path();

        if (fs::is_regular_file(path))
        {
            bool validExtension = false;

            for (const auto& extension : extensions)
            {
                if (path.extension() == extension) 
                {
                    validExtension = true;
                    break;
                }
            }

            if (validExtension) entries.push_back({ path.stem().string(), path.string() });
        }
    }

    return entries;
}