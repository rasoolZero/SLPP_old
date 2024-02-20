#pragma once
#include <array>
#include <vector>
#include "Lights.h"
#include "MIDI.h"
class LightState :  public std::array<Lights,64>
{
public:
	LightState(MIDI& midi);
	void update(const std::vector<Lights>* newFrame);

protected:

private:
	void animateMidi();
	MIDI& midi;
	std::vector<unsigned char> v = { 0,0,0 };
};

