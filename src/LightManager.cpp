#include "LightManager.h"

LightManager::LightManager(MIDI & _midi) : midi(_midi)
{

    lights.resize(16);
    lightHold.resize(16);
    for(int i=0;i<16;i++){
        lights[i].resize(64);
        for(int j=0;j<64;j++)
            lightHold[i].push_back(false);
    }
}
void LightManager::trigger(int pn,int row,int col,bool down){
    int index = row*8+col;
    if(down || (!down && !lightHold[pn][index]))
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

void LightManager::setHold(int pn,int index,bool hold){
    lightHold[pn][index] = hold;
}
bool LightManager::getHold(int pn,int index){
    return lightHold[pn][index];
}

void LightManager::reset(int pn,int index){
    lights[pn][index].clear();
}

int LightManager::getLightCount(int pn,int row,int col){
    int index = row*8+col;
    return lights[pn][index].size();
}
