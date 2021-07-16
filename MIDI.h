#ifndef MIDI_H
#define MIDI_H
#include "RtMidi.h"
#include <memory>
class MIDI
{
    public:
        MIDI();
    protected:

    private:
        std::unique_ptr<RtMidiIn> midiin;
};
void midiInput( double deltatime, std::vector< unsigned char > *message, void *userData);

#endif // MIDI_H
