#ifndef AUDIO_H
#define AUDIO_H
#include <SFML/Audio.hpp>

class Audio
{
    public:
        Audio();
        void load(std::string & address);
        void trigger(bool down);

    protected:

    private:
        sf::Sound sound;
        sf::SoundBuffer buffer;
        bool loaded=false;
};

#endif // AUDIO_H
