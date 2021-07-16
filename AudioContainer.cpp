#include "AudioContainer.h"

AudioContainer::AudioContainer()
{
    for(int i=0;i<16;i++)
        sounds.push_back(std::vector<Audio>(64));
}


void AudioContainer::trigger(int row,int col,bool down){
    int index = row*8+col;
    sounds[pageNumber][index].trigger(down);
}

void AudioContainer::load(int row,int col,std::string & address){
    int index = row*8+col;
    sounds[pageNumber][index].load(address);
}
