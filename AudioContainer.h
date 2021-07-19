#ifndef AUDIOCONTAINER_H
#define AUDIOCONTAINER_H
#include <vector>
#include <Audio.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

class AudioContainer
{
    public:
        AudioContainer();
        ~AudioContainer();
        void setPageNumber(int pn){pageNumber=pn;}
        void trigger(int row,int col,bool down);
        void load(int row,int col,std::string address);
    protected:

    private:
        std::vector<std::vector<Audio>> sounds;
        int pageNumber;
};

#endif // AUDIOCONTAINER_H
