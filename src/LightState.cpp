#include "LightState.h"


LightState::LightState(MIDI& _midi) : midi(_midi), lock(1)
{
}


void LightState::update(const std::vector<Lights>* newFrame)
{
	lock.acquire();
	for (int i = 0; i < 64; i++) {
		if (newFrame->operator[](i) == Lights::Nothing)
			continue;
		this->operator[](i) = newFrame->operator[](i);
	}
	animateMidi();
}

void LightState::animateMidi()
{
	for (int i = 0; i < 64; i++) {
		v = { 146,static_cast<unsigned char>(this->operator[](i)), static_cast<unsigned char>(this->operator[](i + 1)) };
		i++;
		midi.sendCustomMessage(v);
	}
	v = { 128 ,120,0 };
	midi.sendCustomMessage(v);
	lock.release();
}
