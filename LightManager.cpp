#include "LightManager.h"

LightManager::LightManager(MIDI & _midi) : midi(_midi)
{

    lights.resize(16);
    for(int i=0;i<16;i++){
        lights[i].resize(64);
    }
    setLight(0,0,0,4,62);
    setLight(0,0,0,5,63);
    setLight(0,0,0,6,60);
    setLight(0,0,0,7,15);
}
void LightManager::trigger(int pn,int row,int col,bool down){
    int index = row*8+col;
    midi.updateLights(lights[pn][index],down);
}

void LightManager::setLight(int pn,int row,int col,int buttonIndex,int light){
    int index = row*8+col;
    if (light == 12){
        lights[pn][index].erase(buttonIndex);
        return;
    }
    lights[pn][index][buttonIndex] = light;
}

int LightManager::getLight(int pn,int row,int col,int buttonIndex){
    int index = row*8+col;
    if (lights[pn][index].find(buttonIndex) == lights[pn][index].end())
        return 12;
    return lights[pn][index][buttonIndex];
}
