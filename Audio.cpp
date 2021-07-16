#include "Audio.h"

Audio::Audio()
{
    sound.setBuffer(buffer);
}

void Audio::load(std::string & address){
    loaded = buffer.loadFromFile(address);
}

void Audio::trigger(bool down){
    if(!loaded)
        return;
    bool looping = sound.getLoop();
    bool isPlaying = (sound.getStatus() == sf::Sound::Status::Playing);
    if(down){
        if(isPlaying && looping)
            sound.stop();
        else
            sound.play();
    }
}
