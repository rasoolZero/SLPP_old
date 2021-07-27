#include "LightManager.h"

LightManager::LightManager(MIDI & _midi) : midi(_midi)
{

    lights.resize(16);
    for(int i=0;i<16;i++){
        lights[i].resize(64);
    }
}
void LightManager::trigger(int pn,int row,int col,bool down){
    int index = row*8+col;
    //midi now plays lights[pn][index];
}

void LightManager::setLight(int pn,int row,int col,int buttonIndex,int light){
    int index = row*8+col;
    lights[pn][index][buttonIndex] = light;
}
