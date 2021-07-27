#ifndef AUDIOCONTAINER_H
#define AUDIOCONTAINER_H
#include <vector>
#include "Audio.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

class AudioContainer
{
    public:
        AudioContainer();
        ~AudioContainer();
        void trigger(int pn,int row,int col,bool down);
        void stopAll();
        void load(int pn,int row,int col,std::string address);
        Audio * getSound(int pn,int index);

    protected:

    private:
        std::vector<std::vector<Audio>> sounds;
};

#endif // AUDIOCONTAINER_H
