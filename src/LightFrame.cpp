#include "LightFrame.h"

LightFrame::LightFrame(sf::Time duration) {
	this->resize(64);
	this->duration = duration;
}

void LightFrame::animateMidi(MIDI& midi) {
	for (int i = 0; i < 64; i++) {
		v = { 146,static_cast<unsigned char>((*this)[i]), static_cast<unsigned char>((*this)[i + 1]) };
		i++;
		midi.sendCustomMessage(v);
	}
	v = { 128 ,0,0 };
	midi.sendCustomMessage(v);
}