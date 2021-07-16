#ifndef AUDIOCONTAINER_H
#define AUDIOCONTAINER_H
#include <vector>
#include <Audio.h>

class AudioContainer
{
    public:
        AudioContainer();
        void setPageNumber(int pn){pageNumber=pn;}
        void trigger(int row,int col,bool down);
        void load(int row,int col,std::string address);
    protected:

    private:
        std::vector<std::vector<Audio>> sounds;
        int pageNumber;
};

#endif // AUDIOCONTAINER_H
