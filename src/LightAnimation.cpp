#include "LightAnimation.h"

LightAnimation::LightAnimation()
{
	this->resize(1);
}

void LightAnimation::operator()(LightState& launchpadState) {
	sf::Time totalTime = sf::Time::Zero;
	for (size_t i = 0; i < this->size(); i++) {
		LightFrame& iteratedFrame = this->operator[](i);
		func = std::bind(&LightFrame::animateLaunchpad, iteratedFrame, std::placeholders::_1);
		this->setTimeOut<std::reference_wrapper<LightState>>(static_cast<int32_t>(totalTime.asMilliseconds()), func, std::reference_wrapper<LightState>(launchpadState));
		totalTime += iteratedFrame.getDuration();
	}
}