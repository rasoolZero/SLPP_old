#ifndef MIDI_H
#define MIDI_H
#include <RtMidi.h>
#include <memory>
#include <unordered_map>
#include <semaphore>
class Program;
class MIDI
{
    public:
        MIDI(Program & _program);
        ~MIDI();
        int page(std::vector< unsigned char > *message);
        Program & getProgram(){return program;}
        void sendCustomMessage(const std::vector<unsigned char>& message);

        void acquireRapidMode();
        void releaseRapidMode();

    protected:

    private:
        std::binary_semaphore lockRapidMode;
        std::unique_ptr<RtMidiIn> midiin;
        std::unique_ptr<RtMidiOut> midiout;
        Program & program;
        void resetDevice();
};
void midiInput( double deltatime, std::vector< unsigned char > *message, void *userData);

#endif // MIDI_H
