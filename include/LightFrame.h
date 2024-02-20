#pragma once
#include <vector>
#include "Lights.h"
#include "LightState.h"
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

	void animateLaunchpad(LightState& launchpadState);

private:
	sf::Time duration;
};

