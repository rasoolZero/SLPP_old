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

	void animateMidi(MIDI& midi);

private:
	sf::Time duration;
	std::vector<unsigned char> v = { 0,0,0 };
};

