#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H
#include <vector>
#include <unordered_map>
#include "MIDI.h"


class LightManager
{
    public:
        LightManager(MIDI & _midi);


    protected:

    private:

        MIDI & midi;
};

#endif // LIGHTMANAGER_H
