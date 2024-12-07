#pragma once

#include <memory>
#include "resources.hpp"

class SoundPlayer
{
public:
    SoundPlayer(const SoundPlayer&) = delete;
    SoundPlayer& operator=(const SoundPlayer&) = delete;

    static SoundPlayer& get();

    void updateMusic();
    void playMusic(bool value);
    void setMusicVolume(float volume);

    void play(const std::string& name, float volume = 100.f, float pitch = 1.0f);
    void playSounds(bool value);
    void setSoundsVolume(float volume);

private:
    SoundPlayer();
    void cleanSounds();

private:
    bool mPlayMusic;
    sf::Music mMusic;
    std::vector<size_t> mSongIndices;
    std::vector<std::pair<std::string, std::string>> mSongs;
    float mMusicVolume;

    bool mPlaySounds;
    std::vector<std::unique_ptr<sf::Sound>> mSounds;
    float mSoundsVolume;
};