#include "MIDI.h"
#include "Program.h"

MIDI::MIDI(Program & _program) : program(_program)
{
    midiin = std::make_unique<RtMidiIn>();
    midiout = std::make_unique<RtMidiOut>();
    unsigned int nPorts = midiin->getPortCount();
    if(nPorts==0)
        throw std::runtime_error("Error:\nNo MIDI Device found.\nMake sure your Launchpad is connected.");
    midiin->openPort( 0 );
    midiout->openPort( 1 );
    midiin->setCallback(&midiInput,this);
    resetDevice();
}

MIDI::~MIDI(){
    resetDevice();
}
int MIDI::page(std::vector< unsigned char > *message){
    int buttonNumber = message->at(1);
    int data0 = message->at(0);
    if (data0==176)
        return buttonNumber-104;
    if ((buttonNumber-8)%16 == 0)
        return (buttonNumber-8)/16 + 8;
    return -1;
}

void MIDI::sendCustomMessage(const std::vector<unsigned char>& message)
{
    midiout->sendMessage(&message);
}


void MIDI::resetDevice(){
    std::vector<unsigned char> message={176,0,0};
    midiout->sendMessage(&message);
}

void midiInput( double deltatime, std::vector< unsigned char > *message, void *userData){
    bool down = message->at(2)==127;
    int buttonNumber = message->at(1);
    MIDI * midi = (MIDI *) userData;
    int pageNumber = midi->page(message);
    if(pageNumber!=-1){
        midi->getProgram().setPageNumber(pageNumber);
    }
    else{
        int row = buttonNumber/16;
        int col = buttonNumber%8;
        midi->getProgram().trigger(row,col,down);
    }
}
