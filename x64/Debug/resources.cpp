#include "resources.hpp"

Resources::Resources()
: mFonts{}
, mSoundBuffers{}
, mTextures{}
, mShaders{}
{
    for (const auto& entry : getDirectoryEntries(FontsDirectory, { ".ttf" }))
        loadFont(entry.first, entry.second);

    for (const auto& entry : getDirectoryEntries(TexturesDirectory, { ".png" }))
        loadTexture(entry.first, entry.second);

    for (const auto& entry : getDirectoryEntries(SoundsDirectory, { ".wav", ".ogg" }))
        loadSoundBuffer(entry.first, entry.second);

    loadShaders();
}

Resources& Resources::get()
{
    static Resources instance{};
    return instance;
}

sf::Font& Resources::font(const std::string& name)
{
    return mFonts.at(name);
}

sf::SoundBuffer& Resources::soundBuffer(const std::string& name)
{
    return mSoundBuffers.at(name);
}

sf::Texture& Resources::texture(const std::string& name)
{
    return mTextures.at(name);
}

sf::Shader* Resources::shader(const std::string& name)
{
    return mShaders.at(name).get();
}

void Resources::loadFont(const std::string& key, const std::string& filename)
{
    if (mFonts.find(key) != mFonts.end())
        throw std::runtime_error("Unable to load Font from '" + filename + "' because key '" + key + "' already exists");

    mFonts[key] = sf::Font{};
    if (!mFonts[key].loadFromFile(filename))
        throw std::runtime_error("Unable to load Font from '" + filename + "' because loadFromFile method failed");
}

void Resources::loadSoundBuffer(const std::string& key, const std::string& filename)
{
    if(mSoundBuffers.find(key) != mSoundBuffers.end())
        throw std::runtime_error("Unable to load SoundBuffer from '" + filename + "' because key '" + key + "' already exists");

    mSoundBuffers[key] = sf::SoundBuffer{};
    if (!mSoundBuffers[key].loadFromFile(filename))
        throw std::runtime_error("Unable to load SoundBuffer from '" + filename + "' because loadFromFile method failed");
}

void Resources::loadTexture(const std::string& key, const std::string& filename)
{
    if (mTextures.find(key) != mTextures.end())
        throw std::runtime_error("Unable to load Texture from '" + filename + "' because key '" + key + "' already exists");

    mTextures[key] = sf::Texture{};
    if (!mTextures[key].loadFromFile(filename))
        throw std::runtime_error("Unable to load Texture from '" + filename + "' because loadFromFile method failed");
}

void Resources::loadVertexShader(const std::string& key, const std::string& filename)
{
    if (mShaders.find(key) != mShaders.end())
        throw std::runtime_error("Unable to load VertexShader from '" + filename + "' because key '" + key + "' already exists");

    mShaders[key] = std::make_unique<sf::Shader>();
    if (!mShaders[key]->loadFromFile(filename, sf::Shader::Vertex))
        throw std::runtime_error("Unable to load VertexShader from '" + filename + "' because loadFromFile method failed");
}

void Resources::loadFragmentShader(const std::string& key, const std::string& filename)
{
    if (mShaders.find(key) != mShaders.end())
        throw std::runtime_error("Unable to load FragmentShader from '" + filename + "' because key '" + key + "' already exists");

    mShaders[key] = std::make_unique<sf::Shader>();
    if (!mShaders[key]->loadFromFile(filename, sf::Shader::Fragment))
        throw std::runtime_error("Unable to load FragmentShader from '" + filename + "' because loadFromFile method failed");
}

void Resources::loadShader(const std::string& key, const std::string& vertexFilename, const std::string& fragmentFilename)
{
    if (mShaders.find(key) != mShaders.end())
        throw std::runtime_error("Unable to load Shader because key '" + key + "' already exists");

    mShaders[key] = std::make_unique<sf::Shader>();
    if (!mShaders[key]->loadFromFile(vertexFilename, fragmentFilename))
        throw std::runtime_error("Unable to load Shader because loadFromFile method failed");
}

void Resources::loadShaders()
{
    auto vertexEntries = getDirectoryEntries(ShadersDirectory, { ".vert" });
    auto fragmentEntries = getDirectoryEntries(ShadersDirectory, { ".frag" });

    for (const auto& vertex : vertexEntries)
    {
        std::string foundFragment = "";

        for (const auto& fragment : fragmentEntries)
        {
            if (vertex.first == fragment.first)
            {
                foundFragment = fragment.second;
                break;
            }
        }

        if (foundFragment != "")
            loadShader(vertex.first, vertex.second, foundFragment);
        else
            loadVertexShader(vertex.first, vertex.second);
    }

    for (const auto& fragment : fragmentEntries)
    {
        bool foundVertex = false;

        for (const auto& vertex : vertexEntries)
        {
            if (fragment.first == vertex.first)
            {
                foundVertex = true;
                break;
            }
        }

        if (!foundVertex) loadFragmentShader(fragment.first, fragment.second);
    }
}