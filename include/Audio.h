#ifndef AUDIO_H
#define AUDIO_H
#include <SFML/Audio.hpp>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

class Audio
{
    public:
        Audio(int _channelNumber);
        ~Audio();
        int getChannelNumber(){return channelNumber;}
        void load(std::string & address);
        void trigger(bool down);
        bool isLoaded(){return loaded;}
        bool isLooped(){return looping;}
        void stopPlaying();
        void setLooping(bool _looping){looping=_looping;}
        Mix_Chunk * getSample(){return sample;}
        void setSample(Mix_Chunk * _sample){sample=_sample;loaded=true;}
        void clearSample();

    protected:

    private:
        int channelNumber;
        Mix_Chunk *sample=0;
        bool looping=false;
        bool loaded=false;
};

#endif // AUDIO_H
