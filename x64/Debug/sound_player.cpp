#include "sound_player.hpp"

SoundPlayer::SoundPlayer()
: mPlayMusic{ true }
, mMusic{}
, mSongIndices{}
, mSongs{}
, mMusicVolume{ 100.f }
, mPlaySounds{ true }
, mSounds{}
, mSoundsVolume{ 100.f }
{
    for (const auto& entry : getDirectoryEntries(MusicDirectory, { ".ogg" }))
        mSongs.push_back({ entry.first, entry.second });

    for (size_t i = 0; i < mSongs.size(); i++)
        mSongIndices.push_back(i);
}

SoundPlayer& SoundPlayer::get()
{
    static SoundPlayer instance;
    return instance;
}

void SoundPlayer::updateMusic()
{
    auto status = mMusic.getStatus();

    if (!mPlayMusic)
    {
        if(status == sf::SoundSource::Playing) mMusic.stop();
        return;
    }
        
    if (status != sf::SoundSource::Playing)
    {
        if (mSongIndices.size() == 0)
        {
            for (size_t i = 0; i < mSongs.size(); i++)
                mSongIndices.push_back(i);
        }

        size_t songIndex = randomInt(0, mSongIndices.size() - 1);

        if (!mMusic.openFromFile(mSongs[mSongIndices[songIndex]].second))
            throw std::runtime_error("Unable to load Music because openFromFile method failed");

        mMusic.setVolume(0.f);
        mMusic.play();

        mSongIndices.erase(mSongIndices.begin() + songIndex);
    }
    else
    {
        float progress = mMusic.getPlayingOffset() / mMusic.getDuration();

        if (progress < 0.1f)
            mMusic.setVolume(progress * mMusicVolume * 10.f);
        else if (progress > 0.9f)
            mMusic.setVolume((1.0f - progress) * mMusicVolume * 10.f);
        else
            mMusic.setVolume(mMusicVolume);
    }
}

void SoundPlayer::playMusic(bool value)
{
    mPlayMusic = value;
}

void SoundPlayer::setMusicVolume(float volume)
{
    mMusicVolume = volume;
}

void SoundPlayer::play(const std::string& name, float volume, float pitch)
{
    if (!mPlaySounds) return;

    auto sound = std::make_unique<sf::Sound>();
    sound->setBuffer(Resources::get().soundBuffer(name));
    sound->setVolume(volume * mSoundsVolume / 100.f);
    sound->setPitch(pitch);
    sound->play();

    mSounds.push_back(std::move(sound));

    cleanSounds();
}

void SoundPlayer::playSounds(bool value)
{
    mPlaySounds = value;
}

void SoundPlayer::setSoundsVolume(float volume)
{
    mSoundsVolume = volume;
}

void SoundPlayer::cleanSounds()
{
    mSounds.erase(
        std::remove_if(
            mSounds.begin(), 
            mSounds.end(), 
            [](const std::unique_ptr<sf::Sound>& sound) { return sound->getStatus() == sf::SoundSource::Stopped; }
        ), 
        mSounds.end()
    );
}