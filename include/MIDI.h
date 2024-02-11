#ifndef MIDI_H
#define MIDI_H
#include <RtMidi.h>
#include <memory>
#include <unordered_map>
class Program;
class MIDI
{
    public:
        MIDI(Program & _program);
        ~MIDI();
        int page(std::vector< unsigned char > *message);
        Program & getProgram(){return program;}
    protected:

    private:
        std::unique_ptr<RtMidiIn> midiin;
        std::unique_ptr<RtMidiOut> midiout;
        Program & program;
        void resetDevice();
};
void midiInput( double deltatime, std::vector< unsigned char > *message, void *userData);

#endif // MIDI_H
