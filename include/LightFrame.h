#pragma once
#include <vector>
#include "Lights.h"
#include "MIDI.h"
#include <SFML/Graphics.hpp>
class LightFrame : public std::vector<Lights>
{
public:
	LightFrame(sf::Time duration = sf::Time::Zero);


	void setDuration(sf::Time duration) {
		this->duration = duration;
	}
	sf::Time getDuration() {
		return this->duration;
	}

	void animateMidi(MIDI & midi) {
		for (int i = 0; i < 64; i++) {
			v = { 146,static_cast<unsigned char>((*this)[i]), static_cast<unsigned char>((*this)[i + 1]) };
			i++;
			midi.sendCustomMessage(v);
		}
		v = { 128 ,0,0 };
		midi.sendCustomMessage(v);
	}

private:
	sf::Time duration;
	std::vector<unsigned char> v = { 0,0,0 };
};

