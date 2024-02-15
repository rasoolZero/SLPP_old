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
        Lights getFrameLight(int frameIndex, int pageNumber, int buttonIndex, int lightIndex);
        void setFrameLight(int frameIndex, int pageNumber, int buttonIndex, int lightIndex, Lights light);
        void setFrameDuration(int frameIndex, int pageNumber, int buttonIndex, float duration);
        float getFrameDuratoin(int frameIndex, int pageNumber, int buttonIndex);
        size_t getFrameCount(int pageNumber, int buttonIndex);
        int newFrame(int pageNumber, int buttonIndex);

    protected:

    private:
        std::vector<std::vector<LightAnimation>> animations;
        MIDI & midi;
};

#endif // LIGHTMANAGER_H
