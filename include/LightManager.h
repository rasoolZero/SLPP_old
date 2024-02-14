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
        Lights getFrameLight(int frameIndex, int pageNumber, int row, int column, int lightIndex);
        void setFrameLight(int frameIndex, int pageNumber, int row, int column, int lightIndex, Lights light);
        void setFrameDuration(int frameIndex, int pageNumber, int row, int column, float duration);
        float getFrameDuratoin(int frameIndex, int pageNumber, int row, int column);
        size_t getFrameCount(int pageNumber, int row, int column);

    protected:

    private:
        std::vector<std::vector<LightAnimation>> animations;
        MIDI & midi;
};

#endif // LIGHTMANAGER_H
