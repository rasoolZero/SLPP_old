#ifndef MIDI_H
#define MIDI_H
#include "RtMidi.h"
#include <memory>
#include <Program.h>
class MIDI
{
    public:
        MIDI(Program & _program);
        int page(std::vector< unsigned char > *message);
        Program & getProgram(){return program;}
    protected:

    private:
        std::unique_ptr<RtMidiIn> midiin;
        Program & program;
};
void midiInput( double deltatime, std::vector< unsigned char > *message, void *userData);

#endif // MIDI_H
