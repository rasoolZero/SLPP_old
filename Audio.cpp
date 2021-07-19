#include "Audio.h"

Audio::Audio(int _channelNumber)
{
    channelNumber=_channelNumber;
}
Audio::~Audio(){
    if(loaded)
        Mix_FreeChunk(sample);
}

void Audio::load(std::string & address){
    if(loaded)
        Mix_FreeChunk(sample);

    sample=Mix_LoadWAV(address.c_str());
    if(sample!=nullptr) {
        loaded=true;
    }
}

void Audio::trigger(bool down){
    if(!loaded)
        return;
    bool isPlaying = Mix_Playing(channelNumber)==1;
    if(down){
        if(isPlaying && looping)
            Mix_HaltChannel(channelNumber);
        else
            Mix_PlayChannel(channelNumber, sample, looping?-1:0 );
    }
}
