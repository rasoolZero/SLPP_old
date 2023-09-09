#include "AudioContainer.h"


AudioContainer::AudioContainer()
{
    if(SDL_Init(SDL_INIT_AUDIO)==-1) {
        throw std::runtime_error(std::string("Error on SDL_Init\n")+std::string(SDL_GetError()));
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096)==-1) {
        throw std::runtime_error(std::string("Error on Mix_OpenAudio\n")+std::string(Mix_GetError()));
    }
    int flags=MIX_INIT_OGG|MIX_INIT_MP3;
    int initted=Mix_Init(flags);
    if((initted&flags) != flags) {
        throw std::runtime_error(std::string("Error on Mix_init\n")+std::string(Mix_GetError()));
    }
    int channelNumber=0;
    Mix_AllocateChannels(1024);
    Mix_Volume(-1, 128);
    sounds.resize(16);
    for(int i=0;i<16;i++){
        for(int j=0;j<64;j++){
            sounds[i].push_back(Audio(channelNumber));
            channelNumber++;
        }
    }

}
AudioContainer::~AudioContainer(){
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}


void AudioContainer::trigger(int pn,int row,int col,bool down){
    int index = row*8+col;
    sounds[pn][index].trigger(down);
}

void AudioContainer::load(int pn,int row,int col,std::string address){
    int index = row*8+col;
    sounds[pn][index].load(address);
}

void AudioContainer::stopAll(){
    Mix_HaltChannel(-1);
}

Audio * AudioContainer::getSound(int pn,int index){
    return sounds[pn].data()+index;
}
