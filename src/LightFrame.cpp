#include "LightFrame.h"

LightFrame::LightFrame(sf::Time duration) {
	this->resize(64);
	this->duration = duration;
}

void LightFrame::animateLaunchpad(LightState& launchpadState) {
	launchpadState.update(this);
}