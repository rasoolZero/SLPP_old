#include "Audio.h"
Audio::Audio(int _channelNumber)
{
    channelNumber=_channelNumber;
}
Audio::~Audio(){
    clearSample();
}

void Audio::load(std::string & address){
    clearSample();

    sample=Mix_LoadWAV(address.c_str());
    if(sample!=nullptr) {
        loaded=true;
    }
    else{
        throw std::runtime_error(Mix_GetError());
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
    else{
        if(isPlaying && !looping)
            Mix_HaltChannel(channelNumber);
    }
}

void Audio::clearSample(){
    if(loaded){
        Mix_FreeChunk(sample);
        sample=nullptr;
    }
    loaded=false;
    looping=false;
}

void Audio::stopPlaying(){
    Mix_HaltChannel(channelNumber);
}
