#pragma once
#include <vector>
#include "LightFrame.h"
#include <future>
#include <functional>
#include <chrono>
#include "MIDI.h"
class LightAnimation : public std::vector<LightFrame>
{
public:

	void operator()(MIDI& midi) {
		sf::Time totalTime = sf::Time::Zero;
		for (size_t i = 0; i < this->size(); i++) {
			LightFrame& iteratedFrame = this->operator[](i);
			totalTime += iteratedFrame.getDuration();
			func = std::bind(&LightFrame::animateMidi, iteratedFrame, std::placeholders::_1);
			this->setTimeOut<std::reference_wrapper<MIDI>>(static_cast<int32_t>(totalTime.asMilliseconds()), func, std::reference_wrapper<MIDI>(midi));

		}
	}


private:
	std::function<void(std::reference_wrapper<MIDI>)> func = nullptr;

	template <typename... ParamTypes>
	void setTimeOut(int32_t milliseconds, std::function<void(ParamTypes...)> func, ParamTypes... parames) {
		std::thread([=]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
			try {
				func(parames...);
			}
			catch (std::exception& e) {
				std::cout << e.what() << std::endl;
			}
			}).detach();
	};

};

