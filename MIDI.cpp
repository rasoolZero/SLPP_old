#include "MIDI.h"

MIDI::MIDI()
{
    midiin = std::make_unique<RtMidiIn>();
    unsigned int nPorts = midiin->getPortCount();
    if(nPorts==0)
        throw std::runtime_error("No MIDI Device Connected");
    midiin->openPort( 0 );
    midiin->setCallback(&midiInput,this);

}


void midiInput( double deltatime, std::vector< unsigned char > *message, void *userData){
    unsigned int nBytes = message->size();
    for(unsigned int i=0;i<nBytes;i++)
        std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
    std::cout << std::endl;
}
