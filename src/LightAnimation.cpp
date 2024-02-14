#include "LightAnimation.h"

LightAnimation::LightAnimation()
{
	this->resize(1);
}

void LightAnimation::operator()(MIDI& midi) {
	sf::Time totalTime = sf::Time::Zero;
	for (size_t i = 0; i < this->size(); i++) {
		LightFrame& iteratedFrame = this->operator[](i);
		func = std::bind(&LightFrame::animateMidi, iteratedFrame, std::placeholders::_1);
		this->setTimeOut<std::reference_wrapper<MIDI>>(static_cast<int32_t>(totalTime.asMilliseconds()), func, std::reference_wrapper<MIDI>(midi));
		totalTime += iteratedFrame.getDuration();
	}
}