#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H
#include <vector>
#include <unordered_map>
#include "MIDI.h"
#include "LightAnimation.h"


class LightManager
{
    public:
        LightManager(MIDI & _midi);
        void trigger(int pageNumber, int row, int column, bool down);

    protected:

    private:
        std::vector<std::vector<LightAnimation>> animations;
        MIDI & midi;
};

#endif // LIGHTMANAGER_H
