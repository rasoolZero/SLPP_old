#include "AudioContainer.h"

AudioContainer::AudioContainer()
{
    if(SDL_Init(SDL_INIT_AUDIO)==-1) {
        printf("SDL_Init: %s\n", SDL_GetError());
        //stop the program
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 8192)==-1) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        //stop the program
    }
    int flags=MIX_INIT_OGG|MIX_INIT_MOD|MIX_INIT_FLAC|MIX_INIT_MP3;
    int initted=Mix_Init(flags);
    if((initted&flags) != flags) {
        printf("Mix_Init: Failed to init required OGG, mod, Flac and MP3 support!\n");
        printf("Mix_Init: %s\n", Mix_GetError());
        //stop the program
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


void AudioContainer::trigger(int row,int col,bool down){
    int index = row*8+col;
    sounds[pageNumber][index].trigger(down);
}

void AudioContainer::load(int row,int col,std::string address){
    int index = row*8+col;
    sounds[pageNumber][index].load(address);
}

void AudioContainer::stopAll(){
    Mix_HaltChannel(-1);
}
