#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H
#include <vector>
#include <unordered_map>
#include "MIDI.h"


class LightManager
{
    public:
        LightManager(MIDI & _midi);
        void trigger(int pn,int row,int col,bool down);
        void setLight(int pn,int row,int col,int buttonIndex,int light);

    protected:

    private:
        std::vector<std::vector<std::unordered_map<int,int>>> lights;
        MIDI & midi;
};

#endif // LIGHTMANAGER_H
